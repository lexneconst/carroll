#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "commons.h"


// ******************************************************************
//  PUBLIC FUNCTIONS IMPLEMENTATION
// ******************************************************************

void init_tray_icon_data(
   struct tray_icon_data *tid,
   const char *msg,
   struct rgb_color *color,
   const char *tooltip,
   const char *on_click_command)
{
   strncpy(tid->msg, msg, sizeof(tid->msg)-1);
   tid->msg[sizeof(tid->msg)-1] = '\0';
   tid->color = *color;

   strncpy(tid->tooltip, tooltip, sizeof(tid->tooltip)-1);
   tid->tooltip[sizeof(tid->tooltip)-1] = '\0';

   strncpy(tid->on_click_command, on_click_command,sizeof(tid->on_click_command)-1);
   tid->on_click_command[sizeof(tid->on_click_command)-1] = '\0';
}


void prepare_connection(const char *socket_path, int *soc, struct sockaddr_un *addr)
{
   // Prepare socket.
   if ((*soc = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
      perror("Cannot create socket");
      exit(EXIT_FAILURE);
   }

   // Prepare address.
   memset(addr, 0, sizeof(*addr));
   addr->sun_family = AF_UNIX;
   strncpy(addr->sun_path, socket_path, sizeof(addr->sun_path)-1);
}

