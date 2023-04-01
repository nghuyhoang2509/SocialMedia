static GtkBuilder *builder;
static GtkWidget *window;
static GtkWidget *create_a_post_button;
static GtkWidget *cancel_button;
static GtkWidget *profile;
static GtkWidget *create_a_post;
static GtkListBox *list_post;

static void add_post_to_list_box(const char *content);

static void create_a_post_button_click()
{
}

static void cancel_button_click()
{
}

static void add_post_to_list_box(const char *content)
{
    GtkWidget *label_content = gtk_label_new(content);
    GtkWidget *label_test = gtk_label_new("hello");
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    GtkWidget *row = gtk_list_box_row_new();
    GtkStyleContext *context = gtk_widget_get_style_context(row);
    GtkStyleContext *context_label_content = gtk_widget_get_style_context(label_content);
    gtk_style_context_add_class(context, "post");
    gtk_style_context_add_class(context_label_content, "body");

    gtk_label_set_line_wrap(GTK_LABEL(label_content), TRUE);
    gtk_label_set_line_wrap_mode(GTK_LABEL(label_content), PANGO_WRAP_WORD);

    gtk_box_pack_start(GTK_BOX(box), label_test, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label_content, FALSE, FALSE, 0);

    gtk_widget_set_halign(row, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(row, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(row), box);
    gtk_list_box_insert(list_post, row, -1);
}

int Personal()
{

    char data[100];
    sprintf(data, "{\"userId\": \"%s\"}", USER.id);
    printf("Loading personal, Please Wait\n");
    char *response = request("post", data);

    json_object *root = json_tokener_parse(response);
    json_object *post_obj = json_object_object_get(root, "posts");
    int post_array_len = json_object_array_length(post_obj);

    builder = gtk_builder_new_from_file("./pages/Personal/Personal.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "personal"));

    cancel_button = GTK_WIDGET(gtk_builder_get_object(builder, "cancel_button"));

    create_a_post = GTK_WIDGET(gtk_builder_get_object(builder, "create_a_post"));

    profile = GTK_WIDGET(gtk_builder_get_object(builder, "profile"));

    GtkWidget *list_box = GTK_WIDGET(gtk_builder_get_object(builder, "list_box"));
    list_post = GTK_LIST_BOX(list_box);
    for (int i = 0; i < post_array_len; i++)
    {
        json_object *post_string = json_object_array_get_idx(post_obj, i);
        json_object *post_json = json_tokener_parse(json_object_get_string(post_string));
        json_object *content_post;
        json_object_object_get_ex(post_json, "content", &content_post);

        add_post_to_list_box(json_object_get_string(content_post));
        json_object_put(post_string);
    }
    json_object_put(root);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./pages/Personal/style.css", NULL);
    css_set(window, provider);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_window_maximize(GTK_WINDOW(window));
    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
