static GtkWidget *window;

static int log_out_handle()
{
    LOGINED = 0;
    USER.fullname = NULL;
    USER.mail = NULL;
    USER.id = NULL;
    gtk_main_quit();
    gtk_widget_destroy(window);
    PROCESSINIT();
}

static int everyone_handle()
{
    gtk_main_quit();
    gtk_widget_destroy(window);
    printf("Please waiting");
    Everyone();
}

static int discover_handle()
{
    gtk_main_quit();
    gtk_widget_destroy(window);
    printf("Please waiting");
    Discover();
}

int Dashboard()
{
    GtkBuilder *builder = gtk_builder_new_from_file("./pages/Dashboard/Dashboard.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard_page"));

    GtkWidget *discover = GTK_WIDGET(gtk_builder_get_object(builder, "discover"));
    GtkWidget *personal = GTK_WIDGET(gtk_builder_get_object(builder, "personal"));
    GtkWidget *edit_infor = GTK_WIDGET(gtk_builder_get_object(builder, "edit_infor"));
    GtkWidget *everyone = GTK_WIDGET(gtk_builder_get_object(builder, "everyone"));
    GtkWidget *log_out = GTK_WIDGET(gtk_builder_get_object(builder, "log_out"));

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./pages/Dashboard/style.css", NULL);

    css_set(window, provider);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(log_out, "clicked", G_CALLBACK(log_out_handle), NULL);
    g_signal_connect(everyone, "clicked", G_CALLBACK(everyone_handle), NULL);
    g_signal_connect(discover, "clicked", G_CALLBACK(discover_handle), NULL);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}