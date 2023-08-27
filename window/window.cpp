#include "window.hpp"

GtkWidget *CreateWindow(const char *text, gint width, gint height, gboolean resizable, const char *iconPath)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), text);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_window_set_resizable(GTK_WINDOW(window), resizable);
    GError *error = NULL;
    gtk_window_set_icon_from_file(GTK_WINDOW(window), iconPath, &error);
    if (error)
    {
        g_error_free(error);
    }
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    return window;
}

GtkWidget *CreateButton(const char *text, gint width, gint height)
{
    GtkWidget *button = gtk_button_new_with_label(text);
    gtk_widget_set_size_request(button, width, height);
    return button;
}

GtkWidget *CreateLabel(const char *text)
{
    GtkWidget *label = gtk_label_new(text);
    return label;
}

GtkWidget *CreateInput(gint width, gint height, void (*callback)(GtkEntry *, gpointer), gpointer user_data, GList *suggestions)
{
    GtkWidget *input = gtk_entry_new();

    GtkEntryCompletion *completion = gtk_entry_completion_new();

    GtkListStore *completion_model = gtk_list_store_new(1, G_TYPE_STRING);

    GList *iter;
    for (iter = suggestions; iter != NULL; iter = iter->next)
    {
        GtkTreeIter tree_iter;
        gtk_list_store_append(completion_model, &tree_iter);
        gtk_list_store_set(completion_model, &tree_iter, 0, iter->data, -1);
    }

    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(completion_model));
    gtk_entry_completion_set_text_column(completion, 0);

    gtk_entry_set_completion(GTK_ENTRY(input), completion);

    g_signal_connect(input, "activate", G_CALLBACK(callback), user_data);
    gtk_widget_set_size_request(input, width, height);
    return input;
}

GtkWidget *CreateImage(const char *imagePath)
{
    GtkWidget *image = gtk_image_new_from_file(imagePath);
    return image;
}

void ShowPopup(const char *message, const char *iconPath, GtkMessageType type)
{
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               type,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    GError *error = NULL;
    gtk_window_set_icon_from_file(GTK_WINDOW(dialog), iconPath, &error);
    if (error)
    {
        g_error_free(error);
    }
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}