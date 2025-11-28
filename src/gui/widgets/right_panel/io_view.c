#include "gui/gui_widgets.h"

TextScrollInfo createIOView() {
  GtkWidget *text_view = gtk_text_view_new();
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 8);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 8);

  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkWidget *text_scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(text_scroll), text_view);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(text_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);

  // 입력 시 이벤트 조절
  g_signal_connect(buf, "insert-text", G_CALLBACK(onInsertText), NULL);
  g_signal_connect(text_view, "key-press-event", G_CALLBACK(onIOKeyPress),
                   NULL);

  TextScrollInfo info = {text_view, text_scroll};
  return info;
}