#include "gui.h"

GtkWidget *createLeftPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  gtk_box_pack_start(GTK_BOX(box), createFileChooser(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookView(), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), createStepControl(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label("Run"), FALSE,
                     FALSE, 0);

  return box;
}