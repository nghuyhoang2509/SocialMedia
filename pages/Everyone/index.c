static void add_mail_to_list(const char *mail);

static GtkListBox *list_mail;

static void add_mail_to_list(const char *mail)
{
    GtkWidget *label = gtk_label_new(mail);
    GtkWidget *row = gtk_list_box_row_new();
    GtkStyleContext *context = gtk_widget_get_style_context(row);
    gtk_style_context_add_class(context, "container_item");

    gtk_container_add(GTK_CONTAINER(row), label);
    gtk_list_box_insert(list_mail, row, -1);
}

int Everyone()
{
    char data[2];
    sprintf(data, "{}");
    char *response = request("everyone", data);
    json_object *root = json_tokener_parse(response);
    json_object *mail_obj = json_object_object_get(root, "mail");
    int mail_array_len = json_object_array_length(mail_obj);

    GtkBuilder *builder = gtk_builder_new_from_file("./pages/Everyone/Everyone.glade");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "everyone_page"));

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./pages/Everyone/style.css", NULL);

    GtkWidget *label_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_name"));
    GtkWidget *list_box = GTK_WIDGET(gtk_builder_get_object(builder, "list_box"));

    list_mail = GTK_LIST_BOX(list_box);
    for (int i = 0; i < mail_array_len; i++)
    {
        json_object *mail_value = json_object_array_get_idx(mail_obj, i);
        add_mail_to_list(json_object_get_string(mail_value));
    }
    json_object_put(root);

    css_set(window, provider);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(come_back), NULL);
    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}