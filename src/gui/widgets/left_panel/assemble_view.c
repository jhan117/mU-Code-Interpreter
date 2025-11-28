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
  char *text = printAssembleRes();
  insertAtEnd(text_view, text);
  free(text);
}