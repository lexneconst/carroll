#include <stdio.h>
#include "config.h"

int main(int argc, char **argv) {
    const char *message = "Hello world!";

    const char *response = NULL;

    if (argc > 1) {
        response = echo(argv[1]);
    } else {
        response = echo(message);
    }

    printf("ECHO TEST: %s\n", response);

    return 0;
}

