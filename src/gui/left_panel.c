#include "gui.h"

GtkWidget *createLeftPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  NotebookPageInfo pages[] = {
      {"uCode", gtk_text_view_new()},
      {"assemble 결과", gtk_text_view_new()},
      {".lst 결과", gtk_text_view_new()},
  };

  GtkWidget *run_btn = gtk_button_new_with_label("Run");
  gtk_widget_set_halign(run_btn, GTK_ALIGN_CENTER);

  gtk_box_pack_start(GTK_BOX(box), createFileChooser(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookView(pages, 3), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), createStepControl(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), run_btn, FALSE, FALSE, 0);

  return box;
}