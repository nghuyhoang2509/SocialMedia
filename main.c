#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <libgen.h>
#include <math.h>

#include "env.c"
#include "handle.c"

GtkBuilder *builder;
GtkWidget *window;
GtkCssProvider *provider;

int main(int agrc, char *agrv[])
{
    PROCESSINIT();
    return EXIT_SUCCESS;
}
