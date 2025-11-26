#include "gui/gui_widgets.h"

#include "assembler/assemble.h" // printAssembleRes()
#include "core/vm_context.h"

TextScrollInfo createAssembleView() {
  GtkWidget *text_view = gtk_text_view_new();
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 8);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 8);

  GtkWidget *text_scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(text_scroll), text_view);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(text_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

  TextScrollInfo info = {text_view, text_scroll};
  return info;
}

void updateAssembleView() {
  GtkWidget *text_view = getGuiContext()->code_ctx.assemble_view.text_view;

  resetAllText(text_view);
  insertAtEnd(text_view, printAssembleRes());
}

void highlightLine() {
  GuiContext *ctx = getGuiContext();
  GtkWidget *text_view = ctx->code_ctx.assemble_view.text_view;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

  GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
  GtkTextTag *tag = gtk_text_tag_table_lookup(table, "highlight");
  if (!tag) {
    tag = gtk_text_tag_new("highlight");
    g_object_set(tag, "foreground", "red", NULL);
    gtk_text_tag_table_add(table, tag);
  }

  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_remove_tag(buffer, tag, &start, &end);

  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_line(buffer, &iter, getVMContext()->pc);

  GtkTextIter line_end = iter;
  gtk_text_iter_forward_to_line_end(&line_end);
  gtk_text_buffer_apply_tag(buffer, tag, &iter, &line_end);
}