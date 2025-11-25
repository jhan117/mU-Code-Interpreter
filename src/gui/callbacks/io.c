#include "gui/gui_callbacks.h"

#include "assembler/aseemble_utils.h" // isNumber()

void onInsertText(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text,
                  gint len, gpointer user_data) {
  GtkTextIter start = *location;
  GtkTextIter end = *location;

  gtk_text_iter_set_line_offset(&start, 0);
  gtk_text_iter_forward_to_line_end(&end);

  gchar *line_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  if (strncmp(line_text, ">> ", 3) != 0) {
    g_signal_stop_emission_by_name(buffer, "insert-text");
    g_free(line_text);
    return;
  }

  if (g_strcmp0(text, "\n") == 0) {
    const char *num_str = line_text + 3;
    if (isNumber(num_str) && strlen(num_str) > 0) {
      int val = atoi(num_str);
      io_ctx->input_value = val;
      io_ctx->is_input = 1;
    }
  }

  g_free(line_text);
}

void guiIoWrite(int data) {
  GtkWidget *io_view = getGuiContext()->io_ctx.io_view;

  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(io_view));

  char tmp[32];
  if (gtk_text_buffer_get_char_count(buf) == 0) {
    snprintf(tmp, sizeof(tmp), "<< %d ", data);
  } else {
    snprintf(tmp, sizeof(tmp), "\n<< %d ", data);
  }
  insertAtEnd(io_view, tmp);
}

int guiIoRead() {
  GtkWidget *io_view = getGuiContext()->io_ctx.io_view;

  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(io_view));

  if (gtk_text_buffer_get_char_count(buf) == 0) {
    insertAtEnd(io_view, ">> ");
  } else {
    insertAtEnd(io_view, "\n>> ");
  }

  io_ctx->is_input = 0;
  while (!io_ctx->is_input) {
    while (gtk_events_pending())
      gtk_main_iteration();
  }

  return io_ctx->input_value;
}

void guiIoLf() {
  GtkWidget *io_view = getGuiContext()->io_ctx.io_view;
  insertAtEnd(io_view, "\n");
}