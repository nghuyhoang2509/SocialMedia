#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <math.h>
#include <ctype.h>

GtkBuilder *builder;
GtkWidget *window;

int main(int agrc, char *agrv[]){

    gtk_init(&agrc, &agrv);

    builder = gtk_builder_new_from_file("Discover.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "discover"));

    g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit),NULL);

    gtk_builder_connect_signals(builder,NULL);
    gtk_widget_show(window);
    gtk_main();
    return EXIT_SUCCESS;
}
