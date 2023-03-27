#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <libgen.h>
#include <math.h>
#include <stddef.h>

#include "handle.h"
#include "env.c"
#include "pages/index.c"
#include "handle.c"

int main(int agrc, char *agrv[])
{
    gtk_init(&agrc, &agrv);

    PROCESSINIT();

    gtk_main();

    return EXIT_SUCCESS;
}
