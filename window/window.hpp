#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <gtk/gtk.h>

GtkWidget *CreateWindow(const char *text, gint width, gint height, gboolean resizable, const char *iconPath);
GtkWidget *CreateButton(const char *text, gint width, gint height);
GtkWidget *CreateLabel(const char *text);
GtkWidget *CreateInput(gint width, gint height, void (*callback)(GtkEntry *, gpointer), gpointer user_data, GList *suggestions);
GtkWidget *CreateImage(const char *imagePath);
void ShowPopup(const char *message, const char *iconPath, GtkMessageType type);

#endif
