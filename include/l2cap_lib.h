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

#include <stdbool.h>

void l2cap_init(int *argc, char ***argv);
int l2cap_run(void);

bool l2cap_use_quiet(void);
bool l2cap_use_debug(void);

void l2cap_print(const char *format, ...)
				__attribute__((format(printf, 1, 2)));
void l2cap_warn(const char *format, ...)
				__attribute__((format(printf, 1, 2)));

typedef void (*l2cap_destroy_func_t)(void *user_data);
typedef void (*l2cap_data_func_t)(const void *l2cap_data);

void l2cap_add_full(const char *name, const void *l2cap_data,
				l2cap_data_func_t pre_setup_func,
				l2cap_data_func_t setup_func,
				l2cap_data_func_t l2cap_func,
				l2cap_data_func_t teardown_func,
				l2cap_data_func_t post_teardown_func,
				unsigned int timeout,
				void *user_data, l2cap_destroy_func_t destroy);

void l2cap_add(const char *name, const void *l2cap_data,
					l2cap_data_func_t setup_func,
					l2cap_data_func_t l2cap_func,
					l2cap_data_func_t teardown_func);

void *l2cap_get_data(void);

void l2cap_pre_setup_complete(void);
void l2cap_pre_setup_failed(void);

void l2cap_setup_complete(void);
void l2cap_setup_failed(void);

void l2cap_l2cap_passed(void);
void l2cap_l2cap_failed(void);

void l2cap_teardown_complete(void);
void l2cap_teardown_failed(void);

void l2cap_post_teardown_complete(void);
void l2cap_post_teardown_failed(void);

typedef void (*l2cap_wait_func_t)(void *user_data);

void l2cap_wait(unsigned int seconds, l2cap_wait_func_t func,
							void *user_data);
