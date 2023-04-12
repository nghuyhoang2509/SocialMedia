#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <libgen.h>
#include <math.h>
#include <stddef.h>
#include <json-c/json.h>
#include <json-c/arraylist.h>
#include <regex.h>
#include <curl/curl.h>
#include <libwebsockets.h>
#include <pthread.h>

/* #include "socket.c" */
#include "call_API.c"
#include "handle.h"
#include "env.c"
#include "pages/index.c"
#include "handle.c"

int main(int agrc, char *agrv[])
{
    // Socket();
    gtk_init(&agrc, &agrv);

    PROCESSINIT();

    return EXIT_SUCCESS;
}