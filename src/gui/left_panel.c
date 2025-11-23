#include "gui/gui.h"

#include "gui/callbacks.h"

GtkWidget *createLeftPanel() {
  GuiContext *ctx = getGuiContext();
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  GtkWidget *ucode_scroll = initUcodeView();
  TextScrollInfo assemble_info = initAssembleView();
  ctx->assemble_view = assemble_info.text_view;
  gtk_text_view_set_editable(GTK_TEXT_VIEW(assemble_info.text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(assemble_info.text_view),
                                   FALSE);

  // TextScrollInfo lst_info = createTextView();
  // ctx->lst_view = lst_info.text_view;
  // gtk_text_view_set_editable(GTK_TEXT_VIEW(lst_info.text_view), FALSE);
  // gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(lst_info.text_view), FALSE);

  NotebookPageInfo pages[] = {
      {"uCode", ucode_scroll},
      {"assemble 결과", assemble_info.scroll},
      {".lst 결과", gtk_text_view_new()},
  };

  ctx->is_run_done = 0;
  GtkWidget *run_btn = gtk_button_new_with_label("Run");
  gtk_widget_set_halign(run_btn, GTK_ALIGN_CENTER);
  g_signal_connect(run_btn, "clicked", G_CALLBACK(onRun), NULL);

  ctx->file_chooser = createFileChooser();
  g_signal_connect(ctx->file_chooser, "file-set", G_CALLBACK(onFileChosen),
                   NULL);

  gtk_box_pack_start(GTK_BOX(box), ctx->file_chooser, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookView(pages, 3), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), createStepControl(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), run_btn, FALSE, FALSE, 0);

  return box;
}