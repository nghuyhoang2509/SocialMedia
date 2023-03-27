
static char data_mail[4][100] = {"nghuyhoang2509@gmail.com", "nhhhoctap@gmail.com", "crasmaverkgkg@gmail.com", "hoangnhse182096@fpt.edu.vn"};

int Everyone(int agrc, char *agrv[])
{
    char data[2];
    sprintf(data, "{}");
    char *response = request("everyone", data);
    printf("%s", response);

    GtkBuilder *builder = gtk_builder_new_from_file("./pages/Everyone/Everyone.glade");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "everyone_page"));

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./pages/Everyone/style.css", NULL);

    GtkWidget *label_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_name"));
    GtkWidget *list_box = GTK_WIDGET(gtk_builder_get_object(builder, "list_box"));

    GtkListBox *list_mail = GTK_LIST_BOX(list_box);
    for (int i = 0; i <= 4; i++)
    {
        GtkWidget *label = gtk_label_new(data_mail[i]);
        GtkWidget *row = gtk_list_box_row_new();
        GtkStyleContext *context = gtk_widget_get_style_context(row);
        gtk_style_context_add_class(context, "container_item");

        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(list_mail, row, -1);
    }
    gtk_container_foreach(GTK_CONTAINER(list_box), (GtkCallback)css_set, provider);

    css_set(label_name, provider);
    css_set(window, provider);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}