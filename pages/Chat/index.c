#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <libgen.h>
#include <math.h>

GtkBuilder *builder;
GtkWidget *window;
GtkCssProvider *provider;
GtkListBox *listbox;

char data_message[14][100] = {"Hi hello hello hello hello hello hello hello hello hello hello", "Hello", "How are you today", "I'm good, And you", "I'm slepy", "What do you doing", "I play game", "yub, what is your game", "LOL, do you want enjoy with me", "Ok", "What time do you want play", "hmmmm", "Tonight", "20h"};
int data_message_type[14] = {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0};
int n = 14;

static void add_item_to_list_box(char *message, int message_type);

static void btn_send_message_clicked()
{
    add_item_to_list_box("test thoi nha", 1);
}

void css_set(GtkWidget *g_widget, GtkCssProvider *provider)
{
    GtkStyleContext *context = gtk_widget_get_style_context(g_widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void add_item_to_list_box(char *message, int message_type)
{
    GtkWidget *label = gtk_label_new(message);
    GtkWidget *row = gtk_list_box_row_new();
    GtkStyleContext *context = gtk_widget_get_style_context(row);
    gtk_style_context_add_class(context, "container_message");

    if (message_type == 0)
    {
        gtk_widget_set_halign(row, GTK_ALIGN_START);
        gtk_style_context_add_class(context, "partner_message");
    }
    else
    {
        printf("click");

        gtk_widget_set_halign(row, GTK_ALIGN_END);
        gtk_style_context_add_class(context, "my_message");
    }
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(row), label);
    gtk_list_box_insert(listbox, row, -1);
}

int main(int agrc, char *agrv[])
{

    gtk_init(&agrc, &agrv);

    builder = gtk_builder_new_from_file("Chat.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "chat_page"));

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./style.css", NULL);

    GtkWidget *label_mail_partner = GTK_WIDGET(gtk_builder_get_object(builder, "mail_partner"));
    GtkWidget *box = GTK_WIDGET(gtk_builder_get_object(builder, "box"));
    GtkWidget *container_list_message = GTK_WIDGET(gtk_builder_get_object(builder, "container_list_message"));
    GtkWidget *input_message = GTK_WIDGET(gtk_builder_get_object(builder, "input_message"));
    GtkWidget *btn_send_message = GTK_WIDGET(gtk_builder_get_object(builder, "btn_send_message"));

    listbox = GTK_LIST_BOX(container_list_message);
    for (int i = 0; i <= n; i++)
    {
        add_item_to_list_box(data_message[i], data_message_type[i]);
    }
    gtk_container_foreach(GTK_CONTAINER(listbox), (GtkCallback)css_set, provider);

    css_set(label_mail_partner, provider);
    css_set(box, provider);
    css_set(container_list_message, provider);
    css_set(input_message, provider);
    css_set(window, provider);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn_send_message, "clicked", G_CALLBACK(btn_send_message_clicked), NULL);
    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}
