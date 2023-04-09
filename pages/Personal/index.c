static GtkBuilder *builder;
static GtkWidget *window;
static GtkWidget *cancel_button;
static GtkWidget *cancel1_button;
static GtkWidget *create_button;
static GtkWidget *edit_button;
static GtkWidget *profile;
static GtkWidget *create_a_post_btn;
static GtkWidget *dialog_create;
static GtkWidget *dialog_edit;
static GtkListBox *list_post;
static GtkWidget *text_view;
static GtkWidget *text_edit;
static GtkWidget *noti_personal;
static GtkWidget *overlay;

static void add_post_to_list_box(const char *content, const char *ID);
static void on_click_create_button();
static void on_click_edit_button(GtkWidget, gpointer);
static void on_destroy();
static void hide_dialog();
static void create_a_post_button_click();
static void on_delete_button_click(GtkWidget, gpointer);
static void on_edit_button_click(GtkWidget, gpointer);
static void create_a_post_button_click();
static void reset_page();
int Personal();

static gboolean destroy_label(GtkWidget *e)
{
    gtk_widget_destroy(e);
    return G_SOURCE_REMOVE;
}

typedef struct
{
    char ID[100];
    char content[4096];
} Editdata;

static void on_click_create_button()
{
    // GtkLabel *noti_label = GTK_LABEL(gtk_label_new("Post created successfully"));
    // char *markup = "<property name=\"pass-through\">False</property>\n<property name=\"index\">9999</property>";
    // gtk_label_set_markup(GTK_LABEL(noti_label), markup);
    //     // gtk_overlay_add_overlay(GTK_OVERLAY(overlay),GTK_LABEL(noti_label));
    // gtk_container_add(GTK_CONTAINER(overlay),GTK_WIDGET(noti_label));
    // g_timeout_add_seconds(2, (GSourceFunc)destroy_label, GTK_LABEL(noti_label));

    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_bounds(text_buffer, &start_iter, &end_iter);
    char *text = gtk_text_buffer_get_text(text_buffer, &start_iter, &end_iter, FALSE);
    char data[4096];
    sprintf(data, "{\"userId\": \"%s\", \"content\":\"%s\", \"mail\":\"%s\"}", USER.id, text, USER.mail);
    printf("creating post...\n");
    char *response = request("create-post", data);
    g_free(text);
    char *markup = "";
    json_object *root = json_tokener_parse(response);
    if (root == NULL)
    {
        // printf("Loi khi doc JSON\n");
        markup = "<span size='large' background='#FF0000'>Something is wrong, please wait!</span>";
        gtk_label_set_markup(GTK_LABEL(noti_personal), markup);
    }

    const char *error = json_object_get_string(json_object_object_get(root, "error"));
    // printf("error: %s\n", error);
    if (error != NULL)
    {
        json_object_put(root);
        char error_status[1000];
        // printf("have error");
        sprintf(error_status, "<span size='large' background='#FF0000'>%s</span>", error);
        gtk_label_set_markup(GTK_LABEL(noti_personal), error_status);

    }
    else
    {
        // markup = "<span size='large' background='#FF0000'>Post created successfully</span>";
        // gtk_label_set_markup(GTK_LABEL(noti_personal), markup);
        // GtkLabel *noti_label = GTK_LABEL(gtk_label_new("Post created successfully"));
        // // gtk_overlay_add_overlay(GTK_OVERLAY(overlay),GTK_LABEL(noti_label));
        // gtk_container_add(GTK_CONTAINER(overlay),GTK_WIDGET(noti_label));
        // g_timeout_add_seconds(2, (GSourceFunc)destroy_label, GTK_LABEL(noti_label));
        json_object_put(root);
        reset_page();
    }

}


static void on_click_edit_button(GtkWidget e, gpointer id_pointer)
{
    char *id = (char *)id_pointer;
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_edit));
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_bounds(text_buffer, &start_iter, &end_iter);
    char *text = gtk_text_buffer_get_text(text_buffer, &start_iter, &end_iter, FALSE);
    char data[5000];
    sprintf(data, "{\"_id\":\"%s\",\"content\":\"%s\"}", id, text);
    printf("Editing, Please Wait\n");
    printf("%s", data);
    char *response = request("edit-post", data);
    g_free(text);
    char *markup = "";
    json_object *root = json_tokener_parse(response);
    if (root == NULL)
    {
        // printf("Loi khi doc JSON\n");
         markup = "<span size='large' background='#FF0000'>Something is wrong, please wait!</span>";
        gtk_label_set_markup(GTK_LABEL(noti_personal), markup);
    }

    const char *error = json_object_get_string(json_object_object_get(root, "error"));
    // printf("error: %s\n", error);
    if (error != NULL)
    {
        json_object_put(root);
        char error_status[1000];
        // printf("have error");
        sprintf(error_status, "<span size='large' background='#FF0000'>%s</span>", error);
        gtk_label_set_markup(GTK_LABEL(noti_personal), error_status);
    }
    else
    {
        markup = "<span size='large' background='#FF0000'>Post edited successfully</span>";
        gtk_label_set_markup(GTK_LABEL(noti_personal), markup);
        json_object_put(root);
        reset_page();
    }

}

static void reset_page()
{
    gtk_widget_destroy(window);
    gtk_widget_destroy(dialog_create);
    gtk_widget_destroy(dialog_edit);
    Personal();
}

static void on_delete_button_click(GtkWidget e, gpointer id_pointer)
{
    char *id = (char *)id_pointer;
    char data[100];
    sprintf(data, "{\"_id\":\"%s\"}", id);
    printf("Deleting, Please Wait\n");
    char *response = request("delete-post", data);
    
    char *markup = "";
    json_object *root = json_tokener_parse(response);
    if (root == NULL)
    {
        // printf("Loi khi doc JSON\n");
        markup = "<span size='large' background='#FF0000'>Something is wrong, please wait!</span>";
        gtk_label_set_markup(GTK_LABEL(noti_personal), markup);
    }

    const char *error = json_object_get_string(json_object_object_get(root, "error"));
    // printf("error: %s\n", error);
    if (error != NULL)
    {
        json_object_put(root);
        char error_status[1000];
        // printf("have error");
        sprintf(error_status, "<span size='large' background='#FF0000'>%s</span>", error);
        gtk_label_set_markup(GTK_LABEL(noti_personal), error_status);
    }
    else
    {
        markup = "<span size='large' background='#FF0000'>Post deleted successfully</span>";
        gtk_label_set_markup(GTK_LABEL(noti_personal), markup);
        json_object_put(root);
        reset_page();
    }
    
}

static void on_edit_button_click(GtkWidget e, gpointer edit_post_pointer)
{
    Editdata *edit_post = edit_post_pointer;
    char *ID = edit_post->ID;
    char *content = edit_post->content;
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_edit));
    gtk_text_buffer_set_text(text_buffer, content, -1);
    gtk_widget_show_all(dialog_edit);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(on_click_edit_button), (gpointer)ID);
}

static void on_destroy()
{
    gtk_widget_destroy(window);
}

static void hide_dialog()
{
    gtk_widget_hide(dialog_create);
    gtk_widget_hide(dialog_edit);
}

static void create_a_post_button_click()
{
    printf("test");
    // gtk_widget_show_all(dialog_create);
    GtkLabel *noti_label = GTK_LABEL(gtk_label_new("Post created successfully"));
    // char *markup = "<property name=\"pass-through\">False</property>\n<property name=\"index\">9999</property>";
    // gtk_label_set_markup(GTK_LABEL(noti_label), markup);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay),GTK_LABEL(noti_label));
    // gtk_container_add(GTK_CONTAINER(overlay),GTK_WIDGET(noti_label));
}

static void add_post_to_list_box(const char *content, const char *ID)
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
    Editdata *edit_post = malloc(sizeof(Editdata));
    strcpy(edit_post->ID, ID);
    strcpy(edit_post->content, content);
    g_signal_connect(delete_btn, "clicked", G_CALLBACK(on_delete_button_click), (gpointer)ID);
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(on_edit_button_click), (gpointer)edit_post);
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

    dialog_edit = GTK_WIDGET(gtk_builder_get_object(builder, "edit_post_dialog"));

    cancel_button = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel"));

    cancel1_button = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel1"));

    create_button = GTK_WIDGET(gtk_builder_get_object(builder, "btn_create"));

    edit_button = GTK_WIDGET(gtk_builder_get_object(builder, "btn_edit"));

    create_a_post_btn = GTK_WIDGET(gtk_builder_get_object(builder, "create_a_post_button"));

    profile = GTK_WIDGET(gtk_builder_get_object(builder, "profile"));

    text_view = GTK_WIDGET(gtk_builder_get_object(builder, "text_view_post"));

    text_edit = GTK_WIDGET(gtk_builder_get_object(builder, "text_edit_post"));

    noti_personal = GTK_WIDGET(gtk_builder_get_object(builder, "noti_personal"));

    overlay = GTK_WIDGET(gtk_builder_get_object(builder, "overlay"));

    GtkWidget *list_box = GTK_WIDGET(gtk_builder_get_object(builder, "list_box"));
    list_post = GTK_LIST_BOX(list_box);
    for (int i = post_array_len - 1; i >= 0; i--)
    {
        json_object *post_string = json_object_array_get_idx(post_obj, i);
        json_object *post_json = json_tokener_parse(json_object_get_string(post_string));
        json_object *content_post;
        json_object *id_obj;
        json_object *oid_obj;
        json_object_object_get_ex(post_json, "content", &content_post);
        json_object_object_get_ex(post_json, "_id", &id_obj);
        json_object_object_get_ex(id_obj, "$oid", &oid_obj);
        add_post_to_list_box(json_object_get_string(content_post), json_object_get_string(oid_obj));
    }
    json_object_put(root);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./pages/Personal/style.css", NULL);
    css_set(window, provider);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(create_a_post_btn, "clicked", G_CALLBACK(create_a_post_button_click), NULL);
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(hide_dialog), NULL);
    g_signal_connect(cancel1_button, "clicked", G_CALLBACK(hide_dialog), NULL);
    g_signal_connect(create_button, "clicked", G_CALLBACK(on_click_create_button), NULL);
    g_signal_connect(dialog_create, "destroy", G_CALLBACK(on_destroy), NULL);
    g_signal_connect(dialog_edit, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
