#include "gui/gui_widgets.h"

GtkWidget *initIOView() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *io_label = gtk_label_new("입출력");
  gtk_widget_set_halign(io_label, GTK_ALIGN_START);

  GtkWidget *io_view = gtk_text_view_new();
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(io_view));

  // 입력 시 이벤트 조절
  g_signal_connect(buf, "insert-text", G_CALLBACK(onInsertText), NULL);

  gtk_box_pack_start(GTK_BOX(box), io_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), io_view, TRUE, TRUE, 0);

  IOContext *io_ctx = &getGuiContext()->io_ctx;
  io_ctx->io_view = io_view;
  return box;
}