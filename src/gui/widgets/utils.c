#include "gui/gui_widgets.h"

void toggleWidgetsVisible(int is_true) {
  GuiContext *ctx = getGuiContext();

  if (is_true) {
    gtk_widget_set_sensitive(ctx->step_ctx.btn_first, TRUE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_prev, TRUE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_next, TRUE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_last, TRUE);
    gtk_widget_set_sensitive(ctx->run_btn, TRUE);
    for (int i = 0; i < 3; i++) {
      g_object_set(ctx->code_ctx.ucode_table.renderer[i], "editable", TRUE,
                   NULL);
    }
  } else {
    // step 버튼 + run 버튼 비활성화
    gtk_widget_set_sensitive(ctx->step_ctx.btn_first, FALSE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_prev, FALSE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_next, FALSE);
    gtk_widget_set_sensitive(ctx->step_ctx.btn_last, FALSE);
    gtk_widget_set_sensitive(ctx->run_btn, FALSE);

    // ucode table editable 비활성화
    for (int i = 0; i < 3; i++) {
      g_object_set(ctx->code_ctx.ucode_table.renderer[i], "editable", FALSE,
                   NULL);
    }
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