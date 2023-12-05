#ifndef SERVER_H
#define SERVER_H

/*
 * Server side of the app.
 */

#include "commons.h"


void server_run(char *socket_path, struct tray_icon_data *tid);


#endif /* SERVER_H */
