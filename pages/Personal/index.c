static GtkBuilder *builder;
static GtkWidget *window;
static GtkWidget *cancel_button;
static GtkWidget *create_button;
static GtkWidget *profile;
static GtkWidget *create_a_post_btn;
static GtkWidget *dialog_create;
static GtkListBox *list_post;
static GtkWidget *text_view;

static void add_post_to_list_box(const char *content);
static void on_click_create_button();
static void on_destroy();
static void hide_dialog();
static void create_a_post_button_click();
int Personal();

static void on_click_create_button()
{
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_bounds(text_buffer, &start_iter, &end_iter);
    char *text = gtk_text_buffer_get_text(text_buffer, &start_iter, &end_iter, FALSE);
    char data[4096];
    sprintf(data, "{\"userId\": \"%s\", \"content\":\"%s\", \"mail\":\"%s\"}", USER.id, text, USER.mail);
    printf("creating post...\n");
    char *response = request("create-post", data);
    gtk_widget_destroy(window);
    gtk_widget_destroy(dialog_create);
    g_free(text);
    Personal();
}

static void on_destroy()
{
    gtk_widget_destroy(window);
}

static void hide_dialog()
{
    gtk_widget_hide(dialog_create);
}

static void create_a_post_button_click()
{
    gtk_widget_show_all(dialog_create);
}

static void add_post_to_list_box(const char *content)
{
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label_content = gtk_label_new(content);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    GtkWidget *box_group_btn = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    GtkWidget *edit_btn = gtk_button_new_with_label("EDIT");
    GtkWidget *delete_btn = gtk_button_new_with_label("DELETE");

    GtkStyleContext *context = gtk_widget_get_style_context(row);
    GtkStyleContext *context_label_content = gtk_widget_get_style_context(label_content);
    GtkStyleContext *context_edit_btn = gtk_widget_get_style_context(edit_btn);
    GtkStyleContext *context_delete_btn = gtk_widget_get_style_context(delete_btn);
    gtk_style_context_add_class(context, "post");
    gtk_style_context_add_class(context_edit_btn, "h2");
    gtk_style_context_add_class(context_delete_btn, "h2");
    gtk_style_context_add_class(context_label_content, "body");

    gtk_box_set_homogeneous(GTK_BOX(box_group_btn), TRUE);
    gtk_box_pack_start(GTK_BOX(box_group_btn), edit_btn, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box_group_btn), delete_btn, FALSE, TRUE, 0);

    gtk_label_set_line_wrap(GTK_LABEL(label_content), TRUE);
    gtk_label_set_line_wrap_mode(GTK_LABEL(label_content), PANGO_WRAP_WORD);

    gtk_box_pack_start(GTK_BOX(box), box_group_btn, FALSE, FALSE, 0);
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

    dialog_create = GTK_WIDGET(gtk_builder_get_object(builder, "create_post_dialog"));

    cancel_button = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel"));

    create_button = GTK_WIDGET(gtk_builder_get_object(builder, "btn_create"));

    create_a_post_btn = GTK_WIDGET(gtk_builder_get_object(builder, "create_a_post_button"));

    profile = GTK_WIDGET(gtk_builder_get_object(builder, "profile"));

    text_view = GTK_WIDGET(gtk_builder_get_object(builder, "text_view_post"));

    GtkWidget *list_box = GTK_WIDGET(gtk_builder_get_object(builder, "list_box"));
    list_post = GTK_LIST_BOX(list_box);
    for (int i = post_array_len - 1; i >= 0; i--)
    {
        json_object *post_string = json_object_array_get_idx(post_obj, i);
        json_object *post_json = json_tokener_parse(json_object_get_string(post_string));
        json_object *content_post;
        json_object_object_get_ex(post_json, "content", &content_post);

        add_post_to_list_box(json_object_get_string(content_post));
    }
    json_object_put(root);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./pages/Personal/style.css", NULL);
    css_set(window, provider);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(create_a_post_btn, "clicked", G_CALLBACK(create_a_post_button_click), NULL);
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(hide_dialog), NULL);
    g_signal_connect(create_button, "clicked", G_CALLBACK(on_click_create_button), NULL);
    g_signal_connect(dialog_create, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
