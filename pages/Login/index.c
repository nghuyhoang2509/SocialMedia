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
GtkWidget *window;
GtkWidget *sign_in_frame;
GtkWidget *sign_up_frame;
GtkWidget *main_window;
GtkEntry *email_sign_in;
GtkEntry *email_sign_up;
GtkEntry *password_sign_in;
GtkEntry *password_sign_up;
GtkEntry *fullname_sign_up;
GtkWidget *sign_in_btn;
GtkWidget *sign_up_btn;
GtkWidget *sign_up_now_btn;
GtkWidget *sign_in_now_btn;
GtkWidget *text_view;
GtkWidget *main_window;
GtkWidget *sign_in_check;
GtkWidget *sign_up_check;

char *PASSWORD;
char *FULLNAME;
char *EMAIL;
char *ID;
int validate_email(char *email)
{
    char *pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";

    regex_t regex;
    int reti = regcomp(&regex, pattern, REG_EXTENDED);
    reti = regexec(&regex, email, 0, NULL, 0);
    if (!reti)
    {
        return 1;
    }
    else if (reti == REG_NOMATCH)
    {
        return 0;
    }

    regfree(&regex);
}
void on_main_window()
{
    gtk_widget_hide(sign_in_frame);
    gtk_widget_show(main_window);
    // gtk_widget_destroy(sign_in_window);
}
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
void on_sign_in_btn_clicked(GtkWidget *e)
{
    // gchar *t = gtk_entry_get_text(email_sign_in);
    char *markup = "";
    gpointer email = gtk_entry_get_text(email_sign_in);
    gpointer pass = gtk_entry_get_text(password_sign_in);
    if (strlen(email) == 0 || strlen(pass) == 0)
    {
        markup = "<span foreground='#FF0000'>Please, fill in all the information!</span>";
        gtk_label_set_markup(GTK_LABEL(sign_in_check), markup);
        return;
    }
    else if (strlen(pass) < 8)
    {
        markup = "<span foreground='#FF0000'>Password must be 8 characters or more!</span>";
        gtk_label_set_markup(GTK_LABEL(sign_up_check), markup);
        return;
    }
    char data[1000];
    sprintf(data, "{\"mail\":\"%s\", \"password\":\"%s\"}", email, pass);

    char *response = request("login", data);

    json_object *root = json_tokener_parse(response);
    if (root == NULL)
    {
        printf("Loi khi doc JSON\n");
        markup = "<span foreground='#FF0000'>Something is wrong, please wait!</span>";
        gtk_label_set_markup(GTK_LABEL(sign_in_check), markup);
    }
    else
    {
        printf("%s\n", response);
        json_object *id_obj;
        if (json_object_object_get_ex(root, "_id", &id_obj))
        {
            json_object *oid_obj;
            if (json_object_object_get_ex(id_obj, "$oid", &oid_obj))
            {
                ID = json_object_get_string(oid_obj);
                printf("_id: %s\n", ID);
            }
        }

        PASSWORD = json_object_get_string(json_object_object_get(root, "password"));
        printf("password: %s\n", PASSWORD);

        FULLNAME = json_object_get_string(json_object_object_get(root, "fullname"));
        printf("fullname: %s\n", FULLNAME);

        EMAIL = json_object_get_string(json_object_object_get(root, "mail"));
        printf("mail: %s\n", EMAIL);

        const char *error = json_object_get_string(json_object_object_get(root, "error"));
        printf("error: %s\n", error);

        json_object_put(root);

        if (error != NULL)
        {
            char error_status[1000];
            sprintf(error_status, "<span foreground='#FF0000'>%s</span>", error);
            gtk_label_set_markup(GTK_LABEL(sign_in_check), error_status);
        }
        else
        {
            markup = "<span foreground='#00FF00'>Sign in done</span>";
            gtk_label_set_markup(GTK_LABEL(sign_in_check), markup);
            on_main_window();
        }
    }
}
void on_sign_up_btn_clicked(GtkWidget *e)
{
    char *markup = "";
    gpointer fullname = gtk_entry_get_text(fullname_sign_up);
    gpointer email = gtk_entry_get_text(email_sign_up);
    gpointer pass = gtk_entry_get_text(password_sign_up);
    if (strlen(fullname) == 0 || strlen(email) == 0 || strlen(pass) == 0)
    {
        markup = "<span foreground='#FF0000'>Please, fill in all the information!</span>";
        gtk_label_set_markup(GTK_LABEL(sign_up_check), markup);
        return;
    }
    else if(!validate_email(email))
    {
        markup = "<span foreground='#FF0000'>Wrong email format!</span>";
        gtk_label_set_markup(GTK_LABEL(sign_up_check), markup);
        return;
    }
    else if (strlen(pass) < 8)
    {
        markup = "<span foreground='#FF0000'>Password must be 8 characters or more!</span>";
        gtk_label_set_markup(GTK_LABEL(sign_up_check), markup);
        return;
    }

    char data[1000];
    sprintf(data, "{\"mail\":\"%s\", \"password\":\"%s\", \"fullname\":\"%s\"}", email, pass, fullname);

    char *response = request("register", data);

    json_object *root = json_tokener_parse(response);
    if (root == NULL)
    {
        printf("Loi khi doc JSON\n");
        markup = "<span foreground='#FF0000'>Something is wrong, please wait!</span>";
        gtk_label_set_markup(GTK_LABEL(sign_in_check), markup);
    }
    else
    {
        printf("%s\n", response);

        const char *error = json_object_get_string(json_object_object_get(root, "error"));
        printf("error: %s\n", error);

        const char *success = json_object_get_string(json_object_object_get(root, "success"));
        printf("success: %s\n", success);

        json_object_put(root);

        if (error != NULL)
        {
            char error_status[1000];
            sprintf(error_status, "<span foreground='#FF0000'>%s</span>", error);
            gtk_label_set_markup(GTK_LABEL(sign_up_check), error_status);
        }
        else
        {
            char success_status[1000];
            sprintf(success_status, "<span foreground='#00FF00'>%s</span>", success);
            gtk_label_set_markup(GTK_LABEL(sign_up_check), success_status);
        }
    }
}
void on_sign_up_now_btn_clicked(GtkWidget *e)
{
    gtk_widget_hide(sign_in_frame);
    gtk_widget_show(sign_up_frame);
}
void on_sign_in_now_btn_clicked(GtkWidget *e)
{
    gtk_widget_hide(sign_up_frame);
    gtk_widget_show(sign_in_frame);
}
void window_destroy(GtkWidget *w, gpointer window)
{
    gtk_widget_destroy(window);
}
void css_set(GtkCssProvider *cssProvider, GtkWidget *g_widget)
{

    GtkStyleContext *context = gtk_widget_get_style_context(g_widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

int main(int agrc, char *agrv[])
{

    gtk_init(&agrc, &agrv);
    builder = gtk_builder_new_from_file("Login.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    sign_in_frame = GTK_WIDGET(gtk_builder_get_object(builder, "sign_in_frame"));
    sign_up_frame = GTK_WIDGET(gtk_builder_get_object(builder, "sign_up_frame"));
    email_sign_in = GTK_ENTRY(gtk_builder_get_object(builder, "email_sign_in"));
    password_sign_in = GTK_ENTRY(gtk_builder_get_object(builder, "password_sign_in"));
    fullname_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "fullname_sign_up"));
    email_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "email_sign_up"));
    password_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "password_sign_up"));
    sign_in_btn = GTK_WIDGET(gtk_builder_get_object(builder, "sign_in_btn"));
    sign_up_btn = GTK_WIDGET(gtk_builder_get_object(builder, "sign_up_btn"));
    sign_up_now_btn = GTK_WIDGET(gtk_builder_get_object(builder, "sign_up_now_btn"));
    sign_in_now_btn = GTK_WIDGET(gtk_builder_get_object(builder, "sign_in_now_btn"));
    sign_in_check = GTK_WIDGET(gtk_builder_get_object(builder, "sign_in_check"));
    sign_up_check = GTK_WIDGET(gtk_builder_get_object(builder, "sign_up_check"));
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // add css
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "style.css", NULL);
    css_set(css_provider, window);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), window);
    g_signal_connect(main_window, "destroy", G_CALLBACK(window_destroy), main_window);
    g_signal_connect(sign_in_btn, "clicked", G_CALLBACK(on_sign_in_btn_clicked), NULL);
    g_signal_connect(sign_up_btn, "clicked", G_CALLBACK(on_sign_up_btn_clicked), NULL);
    g_signal_connect(sign_up_now_btn, "clicked", G_CALLBACK(on_sign_up_now_btn_clicked), NULL);
    g_signal_connect(sign_in_now_btn, "clicked", G_CALLBACK(on_sign_in_now_btn_clicked), NULL);
    g_signal_connect(password_sign_in, "changed", G_CALLBACK(on_entry_insert_spacebar), NULL);
    g_signal_connect(email_sign_in, "changed", G_CALLBACK(on_entry_insert_spacebar), NULL);
    g_signal_connect(email_sign_up, "changed", G_CALLBACK(on_entry_insert_spacebar), NULL);
    g_signal_connect(password_sign_up, "changed", G_CALLBACK(on_entry_insert_spacebar), NULL);

    gtk_widget_show(window);
    gtk_builder_connect_signals(builder, NULL);
    gtk_main();

    return EXIT_SUCCESS;
}
