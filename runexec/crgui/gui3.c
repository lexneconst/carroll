

#include <stdio.h>
#include <stdlib.h>
#include "libgui.h"

static GtkStatusIcon *tray_icon = NULL;

static void menu_item_callback(GtkMenuItem *widget, gpointer user_data);

static
pointer _icon_from_theme( pointer args) {
	GtkIconTheme *default_theme;
	GtkIconInfo  *icon_info;
	const char   *icon = args;

	printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);

	/* handle gtk theme stuff */
	default_theme = gtk_icon_theme_get_default();
	icon_info = gtk_icon_theme_lookup_icon(default_theme, icon, 24, 0);

	if(icon_info) {
		const char *path = gtk_icon_info_get_filename(icon_info);
		return 0;
	}

	return 1;
}

static
pointer _system( pointer args) {
	int ret;
	pointer arg = args;
	return arg;
}

static
pointer _system_with_reader( pointer args) {
	pointer arg;
	const char *cmd;
	FILE *fd;

	arg = args;
	
	cmd = arg;

	args = args;
	arg  = args;
	
	fd = popen(cmd, "r");
	if(fd) {
		char buf[1024];
		pointer val;

		//while((fgets(buf, sizeof(buf), fd)) != NULL) {
		//	val = s->vptr->mk_string(s, buf);
		//	scheme_call(s, arg, cons(s, val, s->NIL));
		//}

		pclose(fd);
		return NULL;
	}

	g_warning("Unable to execute command: %s\n", cmd);
	return NULL;
}

static
pointer _gui_icon_set( pointer args) {
	int ret;
	pointer arg = args;

	printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
	if(tray_icon) {
		gtk_status_icon_set_visible(tray_icon, FALSE);
		gtk_status_icon_set_from_icon_name(tray_icon, arg);
		gtk_status_icon_set_visible(tray_icon, TRUE);
		return NULL;
	}

	return NULL;
}

static
pointer _gui_tooltip_set(pointer args) {
	int ret;
	pointer arg = args;

	
	if(tray_icon) {
		gui_status_icon_set_tooltip(tray_icon, arg);
		return NULL;
	}

	return NULL;
}

static
void gui_script_init_internals() {
	//SCHEME_DEFINE(s, _icon_from_theme, "icon-from-theme");
	//SCHEME_DEFINE(s, _system, "system");
	//SCHEME_DEFINE(s, _system_with_reader, "system-with-reader");

	/* valid only in GUI (not REPL) */
	//SCHEME_DEFINE(s, _gui_icon_set, "gui-icon");
	//SCHEME_DEFINE(s, _gui_tooltip_set, "gui-tooltip");
}

void *gui_script_init(const char *cfile, GtkStatusIcon *icon) {

	char   *conf_home = NULL;
	FILE   *fd;

	tray_icon = icon;

	/* interpreter boot stuff */
	gui_script_init_internals();

	//if(!cfile) {
	//	conf_home = g_strdup_printf("%s/.onorc", g_get_home_dir());
	//	cfile = conf_home;
	//}

	//fd = fopen(cfile, "r");
	//if(!fd)
	//	g_warning("Unable to read configuration from: %s\n", cfile);
	
	//if(conf_home) g_free(conf_home);
	return NULL;
}

void gui_script_fini() {
	
}

/*
 * to deduplicate, this function will either build menu from *gui-menu* variable
 * or run menu item callback, associated with given label
 */
static GtkWidget *fill_or_run(GtkWidget *menu, const char *find_label) {
	pointer it, user_menu, v, label, icon=NULL;
	GtkWidget *menu_item;
	const char *mlabel;

	    //printf("-create item menu \n");
        printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
		mlabel = "Home";

		if(menu) {
            printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
			menu_item = gtk_image_menu_item_new_with_label(mlabel);
			if(icon != NULL) {
                printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
                //printf("load item menu \n");
				gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item),
											  gtk_image_new_from_file("indicator-messages"));
			}
            printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);
            //printf(":create item menu \n");
			g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(menu_item_callback), menu);
			gui_menu_prepend(GUI_MENU(menu), menu_item);
			gtk_widget_show(menu_item);

		} 
	   // printf("+create item menu \n");
    printf("load icon library %s = %d \n", __FUNCTION__, __LINE__);

	return menu;
}

static
void menu_item_scheme_callback( const char *label) {
	fill_or_run( NULL, label);
}

static
void menu_item_callback(GtkMenuItem *widget, gpointer user_data) {
	const gchar *label = gtk_menu_item_get_label(GTK_MENU_ITEM(widget));
	
	menu_item_scheme_callback((const char*)label);
}

GtkWidget *gui_script_parse_menu( GtkWidget *menu) {
	return fill_or_run( menu, NULL);
}
