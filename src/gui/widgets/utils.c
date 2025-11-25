#include "gui/gui_widgets.h"

void resetAllText(GtkWidget *text_view) {
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  gtk_text_buffer_set_text(buf, "", -1);
}

void insertAtEnd(GtkWidget *text_view, const char *text) {
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buf, &end);
  gtk_text_buffer_insert(buf, &end, text, -1);
}