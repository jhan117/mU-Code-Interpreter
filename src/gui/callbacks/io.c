#include "gui/gui_callbacks.h"

#include "assembler/assemble_utils.h" // isNumber()

static void handleLineInput(IOContext *io_ctx, const gchar *line) {
  gchar *trimmed = g_strstrip((gchar *)line);
  if (isNumber(trimmed) && strlen(trimmed) > 0) {
    g_async_queue_push(io_ctx->input_queue, GINT_TO_POINTER(atoi(trimmed)));
  }
}

void onInsertText(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text,
                  gint len, gpointer user_data) {
  int last_line = gtk_text_buffer_get_line_count(buffer) - 1;
  if (gtk_text_iter_get_line(location) != last_line) {
    g_signal_stop_emission_by_name(buffer, "insert-text");
    return;
  }
}

gboolean onIOKeyPress(GtkWidget *widget, GdkEventKey *event,
                      gpointer user_data) {
  if (!gtk_text_view_get_editable(GTK_TEXT_VIEW(widget))) {
    return TRUE;
  }

  IOContext *io_ctx = &getGuiContext()->io_ctx;

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));

  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(buffer, &iter,
                                   gtk_text_buffer_get_insert(buffer));
  int line = gtk_text_iter_get_line(&iter);

  int last_line = gtk_text_buffer_get_line_count(buffer) - 1;
  // 이전 줄에서 입력 차단
  if (line != last_line) {
    return TRUE;
  }

  int column = gtk_text_iter_get_line_offset(&iter);
  const int prompt_len = 3; // ">> " 길이
  // 앞 삭제 금지
  if ((event->keyval == GDK_KEY_BackSpace || event->keyval == GDK_KEY_Delete) &&
      column <= prompt_len) {
    return TRUE;
  }

  if (event->keyval == GDK_KEY_Return) {
    GtkTextIter start, end;
    gint line_count = gtk_text_buffer_get_line_count(buffer);

    gtk_text_buffer_get_iter_at_line(buffer, &start, line_count - 1);
    end = start;
    gtk_text_iter_forward_to_line_end(&end);

    gchar *line_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    const gchar *prefix = ">> ";
    if (g_str_has_prefix(line_text, prefix)) {
      gchar *user_input = g_strdup(line_text + strlen(prefix));
      handleLineInput(io_ctx, user_input);
      g_free(user_input);
    }

    g_free(line_text);

    insertAtEnd(io_ctx->io_view, "\n>> ");
    return TRUE;
  }
  return FALSE;
}

void guiIoWrite(const char *data) {
  IOContext *io_ctx = &getGuiContext()->io_ctx;

  char tmp[128];
  snprintf(tmp, sizeof(tmp), "%s ", data);

  insertAtEnd(io_ctx->io_view, tmp);
  io_ctx->is_last_write = 1;
}

static gboolean addPromptIdle(gpointer data) {
  IOContext *io_ctx = (IOContext *)data;
  insertAtEnd(io_ctx->io_view, "\n>> ");
  return FALSE;
}

int guiIoRead() {
  IOContext *io_ctx = &getGuiContext()->io_ctx;
  if (io_ctx->is_last_write) {
    g_idle_add(addPromptIdle, io_ctx);
  }

  io_ctx->is_last_write = 0;
  return GPOINTER_TO_INT(g_async_queue_pop(io_ctx->input_queue));
}

void guiIoLf() {
  GtkWidget *io_view = getGuiContext()->io_ctx.io_view;
  insertAtEnd(io_view, "\n");
}