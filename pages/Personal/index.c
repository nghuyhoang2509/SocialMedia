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
GtkWidget *create_a_post_button;
GtkWidget *cancel_button;
GtkWidget *profile;
GtkWidget *create_a_post;

void create_a_post_button_click()
{
    gtk_widget_hide(profile);
    gtk_widget_show(create_a_post);
}

void cancel_button_click()
{
    gtk_widget_hide(create_a_post);
    gtk_widget_show(profile);
}

int main(int agrc, char *agrv[])
{

    gtk_init(&agrc, &agrv);

    builder = gtk_builder_new_from_file("Personal.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "personal"));

    create_a_post_button = GTK_WIDGET(gtk_builder_get_object(builder, "create_a_post_button"));

    cancel_button = GTK_WIDGET(gtk_builder_get_object(builder, "cancel_button"));

    create_a_post = GTK_WIDGET(gtk_builder_get_object(builder, "create_a_post"));

    profile = GTK_WIDGET(gtk_builder_get_object(builder, "profile"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(create_a_post_button, "clicked", G_CALLBACK(create_a_post_button_click), NULL);
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(cancel_button_click), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./style.css", NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_builder_connect_signals(builder, NULL);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_widget_show(window);
    gtk_main();
    return EXIT_SUCCESS;
}
