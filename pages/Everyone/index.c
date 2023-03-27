
static GtkWidget *window;
static GtkListStore *list_store;
GtkTreeIter iter;

static void add_row_to_list_store(char *id, char *mail)
{
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, id, -1);
    gtk_list_store_set(list_store, &iter, 1, mail, -1);
}

int Everyone()
{
    GtkBuilder *builder = gtk_builder_new_from_file("./pages/Everyone/Everyone.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "everyone_page"));
    GtkWidget *label_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_name"));
    GtkTreeView *list_everyone = GTK_TREE_VIEW(gtk_builder_get_object(builder, "list_everyone"));
    list_store = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_store"));
    GtkTreeViewColumn *id_col = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "id_col"));
    GtkTreeViewColumn *mail_col = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "mail_col"));
    GtkCellRenderer *id_row = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "id_row"));
    GtkCellRenderer *mail_row = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "mail_row"));
    GtkTreeSelection *select_list = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "select_list"));
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_tree_view_column_add_attribute(id_col, id_row, "text", 0);
    gtk_tree_view_column_add_attribute(mail_col, mail_row, "text", 1);

    add_row_to_list_store("asdjasdhadshsad", "nghuyhoang2509@gmail.com");
    add_row_to_list_store("asdjasdhadshsadsdasdd", "nghuyhoang2509@gmail.com");
    add_row_to_list_store("asdjasdhadsh", "nghuyhoang2509@gmail.com");
    add_row_to_list_store("asdjasdhadshsaddd", "nghuyhoang2509@gmail.com");

    gtk_css_provider_load_from_path(provider, "./pages/Everyone/style.css", NULL);
    css_set(window, provider);
    css_set(label_name, provider);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    select_list = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_everyone));
    gtk_widget_show_all(window);
    gtk_window_maximize(GTK_WINDOW(window));

    gtk_main();
    return 0;
}