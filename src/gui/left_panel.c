#include "gui/gui.h"

GtkWidget *createLeftPanel() {
  GuiContext *ctx = getGuiContext();

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  TextScrollInfo ucode_info = createTextView();
  ctx->ucode_view = ucode_info.text_view;
  TextScrollInfo assemble_info = createTextView();
  ctx->assemble_view = assemble_info.text_view;
  gtk_text_view_set_editable(GTK_TEXT_VIEW(assemble_info.text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(assemble_info.text_view),
                                   FALSE);

  TextScrollInfo lst_info = createTextView();
  ctx->lst_view = lst_info.text_view;
  gtk_text_view_set_editable(GTK_TEXT_VIEW(lst_info.text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(lst_info.text_view), FALSE);

  NotebookPageInfo pages[] = {
      {"uCode", ucode_info.scroll},
      {"assemble 결과", assemble_info.scroll},
      {".lst 결과", lst_info.scroll},
  };

  GtkWidget *run_btn = gtk_button_new_with_label("Run");
  gtk_widget_set_halign(run_btn, GTK_ALIGN_CENTER);

  ctx->file_chooser = createFileChooser();
  g_signal_connect(ctx->file_chooser, "file-set", G_CALLBACK(onFileChosen),
                   NULL);
  showAssembleResult(assemble_info.text_view);

  gtk_box_pack_start(GTK_BOX(box), ctx->file_chooser, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookView(pages, 3), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), createStepControl(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), run_btn, FALSE, FALSE, 0);

  return box;
}