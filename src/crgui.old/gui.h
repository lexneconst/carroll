#ifndef GUI_H
#define GUI_H

/*
 * Module for handling gui, i.e. tray icon.
 */

#include "commons.h"


typedef void (*gui_start_callback_t)(void);


void gui_start(gui_start_callback_t callback, struct tray_icon_data *tid);

void gui_set(struct tray_icon_data *tid);

void gui_quit(void);


#endif /* GUI_H */
