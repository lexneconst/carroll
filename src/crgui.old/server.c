#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <gtk/gtk.h>

#include "server.h"

#include "commons.h"
#include "gui.h"



typedef void (*callback_t)(struct tray_icon_data *tid);



// ******************************************************************
//  PRIVATE FUNCTIONS IMPLEMENTATION
// ******************************************************************

static void update_trace_icon(struct tray_icon_data *tid)
{
   gui_set(tid);
}


static void socket_listen(char *socket_path, callback_t c)
{
   int soc;
   struct sockaddr_un addr;
   prepare_connection(socket_path, &soc, &addr);

   // Remove socket if it already exists.
   unlink(socket_path);

   // Bind socket to address.
   if (bind(soc, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
      perror("Cannot bind socket to given address");
      exit(EXIT_FAILURE);
   }

   // Start listening.
   // 77 is max number of connections, but this number
   // should not be relevant for unix sockets?
   if (listen(soc, 77) == -1) {
      perror("Cannot start listening");
      exit(EXIT_FAILURE);
   }

   // Start reading.

   int cli;
   struct tray_icon_data buf;

   while (1) {
      // Accept connection, do not care about client's socket struct.
      if ((cli = accept(soc, NULL, NULL)) == -1) {
         perror("Cannot accept connection");
         continue;
      }

      // Read incoming structure.
      ssize_t rc = read(cli, &buf, sizeof(buf));
      close(cli);

      // Check result.
      if (rc == sizeof(buf)) {
         log("read %zu bytes: %s %f\n", rc, buf.msg, buf.color.r);
         c(&buf);
      } else {
         perror("Bad read");
         exit(EXIT_FAILURE);
      }
   }
}


static char *socket_path_global;
static callback_t c_global;

static void socket_listen_lambda(gpointer data)
{
   socket_listen(socket_path_global, c_global);
}


static void start_socket()
{
   // Start listening in new thread, because current thread
   // is used by GTK.
   g_thread_new("dummy", (GThreadFunc)socket_listen_lambda, NULL);
}


// ******************************************************************
//  PUBLIC FUNCTIONS IMPLEMENTATION
// ******************************************************************

void server_run(char *socket_path, struct tray_icon_data *tid)
{
   socket_path_global = socket_path;
   c_global = update_trace_icon;

   gui_start(start_socket, tid);
}

