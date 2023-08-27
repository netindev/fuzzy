#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "internal/commands.hpp"
#include "window/window.hpp"

#define IMAGE "execute.png"
#define FIXED_HEIGHT 35
#define RESIZABLE false

void OnEntryActivate(GtkEntry *entry, gpointer user_data)
{
    const gchar *text = gtk_entry_get_text(entry);
    Process(text, IMAGE);
}

void OnButtonClick(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *text_entry = static_cast<GtkWidget *>(user_data);
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(text_entry));
    Process(text, IMAGE);
}

void CreateWindow(void)
{
    GtkWidget *window = CreateWindow("Fuzzy", 720, FIXED_HEIGHT, RESIZABLE, IMAGE);
    GtkWidget *button = CreateButton("Execute", 100, FIXED_HEIGHT);
    GtkWidget *image = CreateImage(IMAGE);

    GList *suggestions = NULL;
    std::vector<std::string> stringArray = GetFuzzyCommands();
    for (const std::string &str : stringArray)
    {
        suggestions = g_list_append(suggestions, (gpointer)str.c_str());
    }
    GtkWidget *input = CreateInput(650, FIXED_HEIGHT, OnEntryActivate, NULL, suggestions);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);

    gtk_box_pack_start(GTK_BOX(hbox), input, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(button, "clicked", G_CALLBACK(OnButtonClick), input);
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    GdkPixbuf* icon_pixbuf = gdk_pixbuf_new_from_file("execute.png", nullptr);
    CreateWindow();
    gtk_main();
    g_object_unref(icon_pixbuf);
    return 0;
}