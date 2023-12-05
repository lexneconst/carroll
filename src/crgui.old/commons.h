#ifndef COMMONS_H
#define COMMONS_H

/*
 * Module with functionality that is common to both server
 * and client side of app.
 */

#include <sys/un.h>
#include "utils.h"


// Structure for data about what and how show in tray.
struct tray_icon_data {
   char msg[8]; // 8 characters must be enough for anyone!
   struct rgb_color color;
   char tooltip[128];
   char on_click_command[256];
};


void init_tray_icon_data(
   struct tray_icon_data *tid,
   const char *msg,
   struct rgb_color *color,
   const char *tooltip,
   const char *on_click_command);

void prepare_connection(
   const char *socket_path, int *soc, struct sockaddr_un *addr);


#endif /* COMMONS_H */
