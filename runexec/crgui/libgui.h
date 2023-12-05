
#ifndef __GUI_SCRIPT_H__
#define __GUI_SCRIPT_H__

#include <gtk/gtk.h>

typedef void *pointer;

#ifdef USE_GTK3
# define GUI_MENU GTK_MENU_SHELL
# define gui_menu_append  gtk_menu_shell_append
# define gui_menu_prepend gtk_menu_shell_prepend
# define gui_status_icon_set_tooltip gtk_status_icon_set_tooltip_text
#else
# define GUI_MENU GTK_MENU
# define gui_menu_append  gtk_menu_append
# define gui_menu_prepend gtk_menu_prepend
# define gui_status_icon_set_tooltip gtk_status_icon_set_tooltip
#endif


void   *gui_script_init(const char *cfile, GtkStatusIcon *icon);
void    gui_script_fini();

GtkWidget *gui_script_parse_menu( GtkWidget *menu);
#endif
