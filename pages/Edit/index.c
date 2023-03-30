#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <libgen.h>
#include <math.h>
#include <ctype.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <regex.h>
#include "call_API.h"

GtkBuilder *builder;
GtkWidget *edit_window;
GtkEntry *password_edit;
GtkEntry *fullname_edit;
GtkWidget *edit_check;
GtkWidget *save_btn;

char *PASSWORD;
char *FULLNAME;
char *EMAIL;
char *ID = "641dc6146d6d14241c07d5d1";
void on_entry_insert_spacebar(GtkWidget *e)
{
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(e));
    gint length = strlen(text);
    for (gint i = 0; i < length; i++)
    {
        if (text[i] == ' ')
        {
            // Không chấp nhận ký tự Space, xoá nó ra khỏi chuỗi đầu vào
            GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(e));
            gtk_editable_delete_text(GTK_ENTRY(e), i, i + 1);
        }
    }
}

void on_save_btn_clicked(GtkWidget *e)
{
    char *markup = "";
    gpointer fullname = gtk_entry_get_text(fullname_edit);
    gpointer pass = gtk_entry_get_text(password_edit);
    if (strlen(fullname) == 0|| strlen(pass) == 0)
    {
        markup = "<span foreground='#FF0000'>Please, fill in all the information!</span>";
        gtk_label_set_markup(GTK_LABEL(edit_check), markup);
        return;
    }
    else if (strlen(pass) < 8)
    {
        markup = "<span foreground='#FF0000'>Password must be 8 characters or more!</span>";
        gtk_label_set_markup(GTK_LABEL(edit_check), markup);
        return;
    }

    char data[1000];
    sprintf(data, "{\"_id\":\"%s\", \"password\":\"%s\", \"fullname\":\"%s\"}", ID, pass, fullname);

    char *response = request("edit-account", data);
    printf("%s\n",response);
    json_object *root = json_tokener_parse(response);
    if (root == NULL)
    {
        printf("Loi khi doc JSON\n");
        markup = "<span foreground='#FF0000'>Something is wrong, please wait!</span>";
        gtk_label_set_markup(GTK_LABEL(edit_check), markup);
    }
    else
    {
        printf("%s\n", response);

        ID = json_object_get_string(json_object_object_get(root, "_id"));

        FULLNAME = json_object_get_string(json_object_object_get(root, "fullname"));

        PASSWORD = json_object_get_string(json_object_object_get(root, "password"));

        const char *error = json_object_get_string(json_object_object_get(root, "error"));
        printf("error: %s\n", error);

        json_object_put(root);

        if (error != NULL)
        {
            char error_status[1000];
            sprintf(error_status, "<span foreground='#FF0000'>%s</span>", error);
            gtk_label_set_markup(GTK_LABEL(edit_check), error_status);
        }
        else if(ID != NULL)
        {
            char success_status[1000];
            sprintf(success_status, "<span foreground='#00FF00'>%s</span>", "Edit personal information done!");
            gtk_label_set_markup(GTK_LABEL(edit_check), success_status);
        }
    }
}
void css_set(GtkCssProvider *cssProvider, GtkWidget *g_widget)
{

    GtkStyleContext *context = gtk_widget_get_style_context(g_widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

int main(int agrc, char *agrv[])
{

    gtk_init(&agrc, &agrv);
    builder = gtk_builder_new_from_file("Edit.glade");

    edit_window = GTK_WIDGET(gtk_builder_get_object(builder, "edit_window"));
    password_edit = GTK_ENTRY(gtk_builder_get_object(builder, "password_edit"));
    fullname_edit = GTK_ENTRY(gtk_builder_get_object(builder, "fullname_edit"));
    edit_check = GTK_WIDGET(gtk_builder_get_object(builder, "edit_check"));
    save_btn = GTK_WIDGET(gtk_builder_get_object(builder, "save_btn"));

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "style.css", NULL);
    css_set(css_provider, edit_window);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(edit_window, "destroy", G_CALLBACK(gtk_main_quit), edit_window);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_btn_clicked), NULL);
    g_signal_connect(password_edit, "changed", G_CALLBACK(on_entry_insert_spacebar), NULL);
    

    gtk_widget_show(edit_window);
    gtk_builder_connect_signals(builder, NULL);
    gtk_main();

    return EXIT_SUCCESS;
}
