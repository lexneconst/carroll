
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "libgui.h"

#define GUI_VERSON "0.3.0"



/* global interpreter object */

static int  start_repl = 0;
static char *conf_path  = NULL;

static GtkStatusIcon *tray_icon = NULL;

static
void tray_icon_on_click(GtkStatusIcon *status_icon, gpointer user_data) {
	pointer func;
	printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
}

static
void tray_icon_on_menu(GtkStatusIcon *status_icon, guint button,
					   guint activate_time, gpointer menu)
{
	printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL,
				   gtk_status_icon_position_menu,
				   status_icon,
				   0,
				   gtk_get_current_event_time ());
}

static
void menu_exit(GtkWidget *widget, gpointer user_data) {
	printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
	gui_script_fini();
	gtk_main_quit();
}




static
void create_tray_icon(void) {
	GtkWidget *menu, *quit_item, *sep_item;

	/* menu */
	menu = gtk_menu_new();
	quit_item = gtk_image_menu_item_new_with_mnemonic("_Quit");

	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(quit_item),
							  gtk_image_new_from_stock(GTK_STOCK_QUIT, GTK_ICON_SIZE_MENU));

	g_signal_connect(G_OBJECT(quit_item), "activate", G_CALLBACK(menu_exit), NULL);

	gui_menu_append(GUI_MENU(menu), quit_item);
	gtk_widget_show(quit_item);

	sep_item = gtk_separator_menu_item_new();
	gui_menu_prepend(GUI_MENU(menu), sep_item);
	gtk_widget_show(sep_item);

	/* create tray_icon first */
	tray_icon = gtk_status_icon_new();

	/* init interpreter */
	gui_script_init(conf_path, tray_icon);

	/* get user supplied menu */
	menu = gui_script_parse_menu(menu);
	gtk_widget_show(menu);

	/* now setup tray_icon details, as callbacks uses interpreter */
	g_signal_connect(G_OBJECT(tray_icon), "activate",
					 G_CALLBACK(tray_icon_on_click), menu);

	g_signal_connect(G_OBJECT(tray_icon), "popup-menu",
					 G_CALLBACK(tray_icon_on_menu), menu);

	/* default icon, but user can change it */

    //printf("::load icon library \n");
    gtk_status_icon_set_from_file(tray_icon, "docs/icon.png");
    //gtk_status_icon_set_from_icon_name(tray_icon, "indicator-messages");
	gtk_status_icon_set_visible(tray_icon, TRUE);
    //printf("++load icon library \n");
}

static
void parse_args(int argc, char **argv) {
	struct option options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"repl", no_argument, NULL, 'r'},
		{"config", required_argument, NULL, 'c'},
		{NULL, 0, NULL, 0}
	};

	int ch;
	optarg = NULL;
	optind = 0;
	optopt = 0;

	while((ch = getopt_long(argc, argv, "hvrc:", options, NULL)) != EOF) {
		switch(ch) {
			case 'h': {
				gchar *app = g_path_get_basename(argv[0]);
				g_print("Usage: %s [options]\n"
						"Systray access to offlineimap and more.\n"
						 "Options:\n"
						 "   -h, --help             display this help\n"
						 "   -v, --version          display version\n"
						 "   -r, --repl             start program REPL (shell)\n"
						 "   -c, --config [file]    load file instead $HOME/.onorc\n",
						app);
				g_free(app);
				exit(0);
				break;
			}
			case 'v':
				g_print(GUI_VERSON "\n");
				exit(0);
				break;
			case 'r':
				start_repl = 1;
				break;
			case 'c':
				conf_path = optarg;
				break;
			default:
				g_print("Use '--help' to see options.\n");
				exit(0);
				break;
		}
	}
}

int main(int argc, char **argv) {
	parse_args(argc, argv);

	/* Gtk functions are used in Scheme code, so this must be called in case REPL was invoked. */
	gtk_init(&argc, &argv);

	//if(start_repl) {
	//	gui_script_init(conf_path, NULL);

	//	g_print("gui REPL. Type (quit) to exit interpreter.");
		
	//	gui_script_fini();
	//} else {
		create_tray_icon();
		gtk_main();
	//}




	return 0;
}
