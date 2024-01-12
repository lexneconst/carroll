/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2012  Intel Corporation. All rights reserved.
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/signalfd.h>

#include <glib.h>

#include <libcrt.h>

#define COLOR_OFF	"\x1B[0m"
#define COLOR_BLACK	"\x1B[0;30m"
#define COLOR_RED	"\x1B[0;31m"
#define COLOR_GREEN	"\x1B[0;32m"
#define COLOR_YELLOW	"\x1B[0;33m"
#define COLOR_BLUE	"\x1B[0;34m"
#define COLOR_MAGENTA	"\x1B[0;35m"
#define COLOR_CYAN	"\x1B[0;36m"
#define COLOR_WHITE	"\x1B[0;37m"
#define COLOR_HIGHLIGHT	"\x1B[1;39m"

#define print_text(color, fmt, args...) \
		printf(color fmt COLOR_OFF "\n", ## args)

#define print_summary(label, color, value, fmt, args...) \
			printf("%-45s " color "%-10s" COLOR_OFF fmt "\n", \
							label, value, ## args)

#define print_progress(name, color, fmt, args...) \
		printf(COLOR_HIGHLIGHT "%s" COLOR_OFF " - " \
				color fmt COLOR_OFF "\n", name, ## args)

enum l2cap_result {
	L2CAP_RESULT_NOT_RUN,
	L2CAP_RESULT_PASSED,
	L2CAP_RESULT_FAILED,
	L2CAP_RESULT_TIMED_OUT,
};

enum l2cap_stage {
	L2CAP_STAGE_INVALID,
	L2CAP_STAGE_PRE_SETUP,
	L2CAP_STAGE_SETUP,
	L2CAP_STAGE_RUN,
	L2CAP_STAGE_TEARDOWN,
	L2CAP_STAGE_POST_TEARDOWN,
};

struct l2cap_case {
	char *name;
	enum l2cap_result result;
	enum l2cap_stage stage;
	const void *l2cap_data;
	l2cap_data_func_t pre_setup_func;
	l2cap_data_func_t setup_func;
	l2cap_data_func_t l2cap_func;
	l2cap_data_func_t teardown_func;
	l2cap_data_func_t post_teardown_func;
	gdouble start_time;
	gdouble end_time;
	unsigned int timeout;
	unsigned int timeout_id;
	l2cap_destroy_func_t destroy;
	void *user_data;
};

static GMainLoop *main_loop;

static GList *l2cap_list;
static GList *l2cap_current;
static GTimer *l2cap_timer;

static gboolean option_version = FALSE;
static gboolean option_quiet = FALSE;
static gboolean option_debug = FALSE;
static gboolean option_list = FALSE;
static const char *option_prefix = NULL;

static void l2cap_destroy(gpointer data)
{
	struct l2cap_case *l2cap = data;

	if (l2cap->timeout_id > 0)
		g_source_remove(l2cap->timeout_id);

	if (l2cap->destroy)
		l2cap->destroy(l2cap->user_data);

	g_free(l2cap->name);
	g_free(l2cap);
}

void l2cap_print(const char *format, ...)
{
	va_list ap;

	if (l2cap_use_quiet())
		return;

	printf("  %s", COLOR_WHITE);
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("%s\n", COLOR_OFF);
}

void l2cap_warn(const char *format, ...)
{
	va_list ap;

	printf("  %s", COLOR_WHITE);
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("%s\n", COLOR_OFF);
}

static void default_pre_setup(const void *l2cap_data)
{
	l2cap_pre_setup_complete();
}

static void default_setup(const void *l2cap_data)
{
	l2cap_setup_complete();
}

static void default_teardown(const void *l2cap_data)
{
	l2cap_teardown_complete();
}

static void default_post_teardown(const void *l2cap_data)
{
	l2cap_post_teardown_complete();
}

void l2cap_add_full(const char *name, const void *l2cap_data,
				l2cap_data_func_t pre_setup_func,
				l2cap_data_func_t setup_func,
				l2cap_data_func_t l2cap_func,
				l2cap_data_func_t teardown_func,
				l2cap_data_func_t post_teardown_func,
				unsigned int timeout,
				void *user_data, l2cap_destroy_func_t destroy)
{
	struct l2cap_case *l2cap;

	if (!l2cap_func)
		return;

	if (option_prefix && !g_str_has_prefix(name, option_prefix)) {
		if (destroy)
			destroy(user_data);
		return;
	}

	if (option_list) {
		printf("%s\n", name);
		if (destroy)
			destroy(user_data);
		return;
	}

	l2cap = g_new0(struct l2cap_case, 1);

	l2cap->name = g_strdup(name);
	l2cap->result = L2CAP_RESULT_NOT_RUN;
	l2cap->stage = L2CAP_STAGE_INVALID;

	l2cap->l2cap_data = l2cap_data;

	if (pre_setup_func)
		l2cap->pre_setup_func = pre_setup_func;
	else
		l2cap->pre_setup_func = default_pre_setup;

	if (setup_func)
		l2cap->setup_func = setup_func;
	else
		l2cap->setup_func = default_setup;

	l2cap->l2cap_func = l2cap_func;

	if (teardown_func)
		l2cap->teardown_func = teardown_func;
	else
		l2cap->teardown_func = default_teardown;

	if (post_teardown_func)
		l2cap->post_teardown_func = post_teardown_func;
	else
		l2cap->post_teardown_func = default_post_teardown;

	l2cap->timeout = timeout;

	l2cap->destroy = destroy;
	l2cap->user_data = user_data;

	l2cap_list = g_list_append(l2cap_list, l2cap);
}

void l2cap_add(const char *name, const void *l2cap_data,
					l2cap_data_func_t setup_func,
					l2cap_data_func_t l2cap_func,
					l2cap_data_func_t teardown_func)
{
	l2cap_add_full(name, l2cap_data, NULL, setup_func, l2cap_func,
					teardown_func, NULL, 0, NULL, NULL);
}

void *l2cap_get_data(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return NULL;

	l2cap = l2cap_current->data;

	return l2cap->user_data;
}

static void l2cap_summarize(void)
{
	unsigned int not_run = 0, passed = 0, failed = 0;
	gdouble execution_time;
	GList *list;

	printf("\n");
	print_text(COLOR_HIGHLIGHT, "");
	print_text(COLOR_HIGHLIGHT, "l2cap Summary");
	print_text(COLOR_HIGHLIGHT, "------------");

	for (list = g_list_first(l2cap_list); list; list = g_list_next(list)) {
		struct l2cap_case *l2cap = list->data;
		gdouble exec_time;

		exec_time = l2cap->end_time - l2cap->start_time;

		switch (l2cap->result) {
		case L2CAP_RESULT_NOT_RUN:
			print_summary(l2cap->name, COLOR_YELLOW, "Not Run", "");
			not_run++;
			break;
		case L2CAP_RESULT_PASSED:
			print_summary(l2cap->name, COLOR_GREEN, "Passed",
						"%8.3f seconds", exec_time);
			passed++;
			break;
		case L2CAP_RESULT_FAILED:
			print_summary(l2cap->name, COLOR_RED, "Failed",
						"%8.3f seconds", exec_time);
			failed++;
			break;
		case L2CAP_RESULT_TIMED_OUT:
			print_summary(l2cap->name, COLOR_RED, "Timed out",
						"%8.3f seconds", exec_time);
			failed++;
			break;
		}
        }

	printf("\nTotal: %d, "
		COLOR_GREEN "Passed: %d (%.1f%%)" COLOR_OFF ", "
		COLOR_RED "Failed: %d" COLOR_OFF ", "
		COLOR_YELLOW "Not Run: %d" COLOR_OFF "\n",
			not_run + passed + failed, passed,
			(float) passed * 100 / (not_run + passed + failed),
			failed, not_run);

	execution_time = g_timer_elapsed(l2cap_timer, NULL);
	printf("Overall execution time: %.3g seconds\n", execution_time);

}

static gboolean teardown_callback(gpointer user_data)
{
	struct l2cap_case *l2cap = user_data;

	l2cap->stage = L2CAP_STAGE_TEARDOWN;

	print_progress(l2cap->name, COLOR_MAGENTA, "teardown");
	l2cap->teardown_func(l2cap->l2cap_data);

	return FALSE;
}

static gboolean l2cap_timeout(gpointer user_data)
{
	struct l2cap_case *l2cap = user_data;

	l2cap->timeout_id = 0;

	if (!l2cap_current)
		return FALSE;

	l2cap->result = L2CAP_RESULT_TIMED_OUT;
	print_progress(l2cap->name, COLOR_RED, "l2cap timed out");

	g_idle_add(teardown_callback, l2cap);

	return FALSE;
}

static void next_l2cap_case(void)
{
	struct l2cap_case *l2cap;

	if (l2cap_current)
		l2cap_current = g_list_next(l2cap_current);
	else
		l2cap_current = l2cap_list;

	if (!l2cap_current) {
		g_timer_stop(l2cap_timer);

		g_main_loop_quit(main_loop);
		return;
	}

	l2cap = l2cap_current->data;

	printf("\n");
	print_progress(l2cap->name, COLOR_BLACK, "init");

	l2cap->start_time = g_timer_elapsed(l2cap_timer, NULL);

	if (l2cap->timeout > 0)
		l2cap->timeout_id = g_timeout_add_seconds(l2cap->timeout,
							l2cap_timeout, l2cap);

	l2cap->stage = L2CAP_STAGE_PRE_SETUP;

	l2cap->pre_setup_func(l2cap->l2cap_data);
}

static gboolean setup_callback(gpointer user_data)
{
	struct l2cap_case *l2cap = user_data;

	l2cap->stage = L2CAP_STAGE_SETUP;

	print_progress(l2cap->name, COLOR_BLUE, "setup");
	l2cap->setup_func(l2cap->l2cap_data);

	return FALSE;
}

static gboolean run_callback(gpointer user_data)
{
	struct l2cap_case *l2cap = user_data;

	l2cap->stage = L2CAP_STAGE_RUN;

	print_progress(l2cap->name, COLOR_BLACK, "run");
	l2cap->l2cap_func(l2cap->l2cap_data);

	return FALSE;
}

static gboolean done_callback(gpointer user_data)
{
	struct l2cap_case *l2cap = user_data;

	l2cap->end_time = g_timer_elapsed(l2cap_timer, NULL);

	print_progress(l2cap->name, COLOR_BLACK, "done");
	next_l2cap_case();

	return FALSE;
}

void l2cap_pre_setup_complete(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_PRE_SETUP)
		return;

	g_idle_add(setup_callback, l2cap);
}

void l2cap_pre_setup_failed(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_PRE_SETUP)
		return;

	l2cap->stage = L2CAP_STAGE_SETUP;

	l2cap_setup_failed();
}

void l2cap_setup_complete(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_SETUP)
		return;

	print_progress(l2cap->name, COLOR_BLUE, "setup complete");

	g_idle_add(run_callback, l2cap);
}

void l2cap_setup_failed(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_SETUP)
		return;

	print_progress(l2cap->name, COLOR_RED, "setup failed");

	g_idle_add(done_callback, l2cap);
}

void l2cap_l2cap_passed(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_RUN)
		return;

	if (l2cap->timeout_id > 0) {
		g_source_remove(l2cap->timeout_id);
		l2cap->timeout_id = 0;
	}

	l2cap->result = L2CAP_RESULT_PASSED;
	print_progress(l2cap->name, COLOR_GREEN, "l2cap passed");

	g_idle_add(teardown_callback, l2cap);
}

void l2cap_l2cap_failed(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_RUN)
		return;

	if (l2cap->timeout_id > 0) {
		g_source_remove(l2cap->timeout_id);
		l2cap->timeout_id = 0;
	}

	l2cap->result = L2CAP_RESULT_FAILED;
	print_progress(l2cap->name, COLOR_RED, "l2cap failed");

	g_idle_add(teardown_callback, l2cap);
}

void l2cap_teardown_complete(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_TEARDOWN)
		return;

	l2cap->stage = L2CAP_STAGE_POST_TEARDOWN;

	l2cap->post_teardown_func(l2cap->l2cap_data);
}

void l2cap_teardown_failed(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_TEARDOWN)
		return;

	l2cap->stage = L2CAP_STAGE_POST_TEARDOWN;

	l2cap_post_teardown_failed();
}

void l2cap_post_teardown_complete(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_POST_TEARDOWN)
		return;

	print_progress(l2cap->name, COLOR_MAGENTA, "teardown complete");

	g_idle_add(done_callback, l2cap);
}

void l2cap_post_teardown_failed(void)
{
	struct l2cap_case *l2cap;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	if (l2cap->stage != L2CAP_STAGE_POST_TEARDOWN)
		return;

	print_progress(l2cap->name, COLOR_RED, "teardown failed");

	g_idle_add(done_callback, l2cap);
}

static gboolean start_l2cap(gpointer user_data)
{
	l2cap_timer = g_timer_new();

	next_l2cap_case();

	return FALSE;
}

struct wait_data {
	unsigned int seconds;
	struct l2cap_case *l2cap;
	l2cap_wait_func_t func;
	void *user_data;
};

static gboolean wait_callback(gpointer user_data)
{
	struct wait_data *wait = user_data;
	struct l2cap_case *l2cap = wait->l2cap;

	wait->seconds--;

	if (wait->seconds > 0) {
		print_progress(l2cap->name, COLOR_BLACK, "%u seconds left",
								wait->seconds);
		return TRUE;
	}

	print_progress(l2cap->name, COLOR_BLACK, "waiting done");

	wait->func(wait->user_data);

	g_free(wait);

	return FALSE;
}

void l2cap_wait(unsigned int seconds, l2cap_wait_func_t func,
							void *user_data)
{
	struct l2cap_case *l2cap;
	struct wait_data *wait;

	if (!func || seconds < 1)
		return;

	if (!l2cap_current)
		return;

	l2cap = l2cap_current->data;

	print_progress(l2cap->name, COLOR_BLACK, "waiting %u seconds", seconds);

	wait = g_new0(struct wait_data, 1);

	wait->seconds = seconds;
	wait->l2cap = l2cap;
	wait->func = func;
	wait->user_data = user_data;

	g_timeout_add(1000, wait_callback, wait);
}

static gboolean signal_handler(GIOChannel *channel, GIOCondition condition,
							gpointer user_data)
{
	static unsigned int __terminated = 0;
	struct signalfd_siginfo si;
	ssize_t result;
	int fd;

	if (condition & (G_IO_NVAL | G_IO_ERR | G_IO_HUP)) {
		g_main_loop_quit(main_loop);
		return FALSE;
	}

	fd = g_io_channel_unix_get_fd(channel);

	result = read(fd, &si, sizeof(si));
	if (result != sizeof(si))
		return FALSE;

	switch (si.ssi_signo) {
	case SIGINT:
	case SIGTERM:
		if (__terminated == 0)
			g_main_loop_quit(main_loop);

		__terminated = 1;
		break;
	}

	return TRUE;
}

static guint setup_signalfd(void)
{
	GIOChannel *channel;
	guint source;
	sigset_t mask;
	int fd;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
		perror("Failed to set signal mask");
		return 0;
	}

	fd = signalfd(-1, &mask, 0);
	if (fd < 0) {
		perror("Failed to create signal descriptor");
		return 0;
	}

	channel = g_io_channel_unix_new(fd);

	g_io_channel_set_close_on_unref(channel, TRUE);
	g_io_channel_set_encoding(channel, NULL, NULL);
	g_io_channel_set_buffered(channel, FALSE);

	source = g_io_add_watch(channel,
				G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL,
				signal_handler, NULL);

	g_io_channel_unref(channel);

	return source;
}

bool l2cap_use_quiet(void)
{
	return option_quiet == TRUE ? true : false;
}

bool l2cap_use_debug(void)
{
	return option_debug == TRUE ? true : false;
}

static GOptionEntry options[] = {
	{ "version", 'v', 0, G_OPTION_ARG_NONE, &option_version,
				"Show version information and exit" },
	{ "quiet", 'q', 0, G_OPTION_ARG_NONE, &option_quiet,
				"Run l2cap without logging" },
	{ "debug", 'd', 0, G_OPTION_ARG_NONE, &option_debug,
				"Run l2cap with debug output" },
	{ "list", 'l', 0, G_OPTION_ARG_NONE, &option_list,
				"Only list the l2cap to be run" },
	{ "prefix", 'p', 0, G_OPTION_ARG_STRING, &option_prefix,
				"Run l2cap matching provided prefix" },
	{ NULL },
};

void l2cap_init(int *argc, char ***argv)
{
	GOptionContext *context;
	GError *error = NULL;

	context = g_option_context_new(NULL);
	g_option_context_add_main_entries(context, options, NULL);

	if (g_option_context_parse(context, argc, argv, &error) == FALSE) {
		if (error != NULL) {
			g_printerr("%s\n", error->message);
			g_error_free(error);
		} else
			g_printerr("An unknown error occurred\n");
		exit(1);
	}

	g_option_context_free(context);

	if (option_version == TRUE) {
		g_print("%s\n", VERSION);
		exit(EXIT_SUCCESS);
	}

	main_loop = g_main_loop_new(NULL, FALSE);

	l2cap_list = NULL;
	l2cap_current = NULL;
}

int l2cap_run(void)
{
	guint signal;

	if (!main_loop)
		return EXIT_FAILURE;

	if (option_list) {
		g_main_loop_unref(main_loop);
		return EXIT_SUCCESS;
	}

	signal = setup_signalfd();

	g_idle_add(start_l2cap, NULL);
	g_main_loop_run(main_loop);

	g_source_remove(signal);

	g_main_loop_unref(main_loop);

	l2cap_summarize();

	g_list_free_full(l2cap_list, l2cap_destroy);

	return EXIT_SUCCESS;
}
