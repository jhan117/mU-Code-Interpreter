#include "gui/gui_callbacks.h"

#include "assembler/assemble_utils.h" // isNumber()

// 지우는 이벤트도 없애기
void onInsertText(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text,
                  gint len, gpointer user_data) {
  IOContext *io_ctx = &getGuiContext()->io_ctx;

  GtkTextIter start = *location;
  GtkTextIter end = *location;
  gtk_text_iter_set_line_offset(&start, 0);
  gtk_text_iter_forward_to_line_end(&end);

  gchar *line_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  if (!line_text)
    return;

  if (strncmp(line_text, ">> ", 3) != 0) {
    g_signal_stop_emission_by_name(buffer, "insert-text");
    g_free(line_text);
    return;
  }

  if (g_strcmp0(text, "\n") == 0) {
    const char *num_str = line_text + 3;
    if (isNumber(num_str) && strlen(num_str) > 0) {
      int val = atoi(num_str);
      g_async_queue_push(io_ctx->input_queue, GINT_TO_POINTER(val));
    }
  }

  g_free(line_text);
}

void guiIoWrite(const char *data) {
  GtkWidget *io_view = getGuiContext()->io_ctx.io_view;

  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(io_view));

  char tmp[32];
  snprintf(tmp, sizeof(tmp), "<< %s ", data);
  insertAtEnd(io_view, tmp);
}

static gboolean guiIoPrompt(gpointer data) {
  IOContext *io_ctx = (IOContext *)data;
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(io_ctx->io_view));

  insertAtEnd(io_ctx->io_view, ">> ");

  return G_SOURCE_REMOVE;
}

int guiIoRead() {
  IOContext *io_ctx = &getGuiContext()->io_ctx;

  g_idle_add(guiIoPrompt, io_ctx);

  return GPOINTER_TO_INT(g_async_queue_pop(io_ctx->input_queue));
}

void guiIoLf() {
  GtkWidget *io_view = getGuiContext()->io_ctx.io_view;
  insertAtEnd(io_view, "\n");
}