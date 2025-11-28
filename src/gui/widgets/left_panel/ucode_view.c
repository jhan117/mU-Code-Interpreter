#include "gui/gui_widgets.h"

#include "core/vm_context.h"

TextScrollInfo createUcodeView() {
  GtkWidget *text_view = gtk_text_view_new();
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 8);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 8);

  GtkWidget *text_scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(text_scroll), text_view);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(text_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  TextScrollInfo info = {text_view, text_scroll};
  return info;
}

void updateUcodeView(char *content) {
  GuiContext *ctx = getGuiContext();
  GtkWidget *text_view = ctx->code_ctx.ucode_view.text_view;

  resetAllText(text_view);
  insertAtEnd(text_view, content);

  free(content);
  ctx->step_ctx.current_step = 0;
}

int getUcodeView(char ***lines, int *line_count) {
  GtkWidget *text_view = getGuiContext()->code_ctx.ucode_view.text_view;

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  int total_lines = gtk_text_buffer_get_line_count(buffer);

  *lines = malloc(sizeof(char *) * total_lines);
  if (!*lines)
    return 0;

  *line_count = 0;

  GtkTextIter iter;
  gtk_text_buffer_get_start_iter(buffer, &iter);

  for (int i = 0; i < total_lines; i++) {
    GtkTextIter line_end = iter;
    gtk_text_iter_forward_to_line_end(&line_end);

    char *line = gtk_text_buffer_get_text(buffer, &iter, &line_end, FALSE);
    if (!line) {
      for (int j = 0; j < *line_count; j++)
        g_free((*lines)[j]);
      free(*lines);
      *lines = NULL;
      *line_count = 0;
      return 0;
    }

    (*lines)[*line_count] = line; // 위젯꺼니까 g_free로 나중에 해제
    (*line_count)++;

    gtk_text_iter_forward_line(&iter);
  }

  return 1;
}

// 메모리 해제
void freeUcoView(char **lines, int line_count) {
  if (!lines)
    return;

  for (int i = 0; i < line_count; i++) {
    g_free(lines[i]);
  }
  free(lines);
}