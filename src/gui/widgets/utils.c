#include "gui/gui_widgets.h"

void toggleWidgetsVisible(int is_true) {
  GuiContext *ctx = getGuiContext();

  if (is_true) {
    gtk_widget_set_sensitive(ctx->step_ctx.btn_first, TRUE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_prev, TRUE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_next, TRUE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_last, TRUE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_toggle, TRUE);
    gtk_widget_set_sensitive(ctx->run_btn, TRUE);
    gtk_text_view_set_editable(ctx->code_ctx.ucode_view.text_view, TRUE);
  } else {
    // step 버튼 + run 버튼 비활성화 + ucode 뷰 비활성화
    gtk_widget_set_sensitive(ctx->step_ctx.btn_first, FALSE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_prev, FALSE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_next, FALSE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_last, FALSE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_toggle, FALSE);
    gtk_widget_set_sensitive(ctx->run_btn, FALSE);
    gtk_text_view_set_editable(ctx->code_ctx.ucode_view.text_view, FALSE);
  }
}

void resetAllText(GtkWidget *text_view) {
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  gtk_text_buffer_set_text(buf, "", -1);
}

void insertAtEnd(GtkWidget *text_view, const char *text) {
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buf, &end);
  g_signal_handlers_block_by_func(buf, onInsertText, NULL);
  gtk_text_buffer_insert(buf, &end, text, -1);
  gtk_text_buffer_get_end_iter(buf, &end);
  g_signal_handlers_unblock_by_func(buf, onInsertText, NULL);
}

void highlightLine(GtkWidget *text_view, int line) {
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

  GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
  GtkTextTag *tag = gtk_text_tag_table_lookup(table, "highlight");
  if (!tag) {
    tag = gtk_text_tag_new("highlight");
    g_object_set(tag, "foreground", "red", NULL);
    gtk_text_tag_table_add(table, tag);
  }

  // 전체 글에서 태그 제거
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_remove_tag(buffer, tag, &start, &end);

  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_line(buffer, &iter, line);

  GtkTextIter line_end = iter;
  gtk_text_iter_forward_to_line_end(&line_end);
  gtk_text_buffer_apply_tag(buffer, tag, &iter, &line_end);
  if (line != -1)
    gtk_text_view_scroll_to_iter(text_view, &iter, 0.49, TRUE, 0.0, 0.0);
}
