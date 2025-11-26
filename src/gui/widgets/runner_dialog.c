#include "gui/gui_widgets.h"

void initRunDialog(RunDialog *rd, GtkWindow *parent, const char *title,
                   const char *initial_text) {

  GtkWidget *dialog = gtk_dialog_new_with_buttons(
      title, parent, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "_OK",
      GTK_RESPONSE_OK, NULL);

  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget *label = gtk_label_new(initial_text);
  GtkWidget *spinner = gtk_spinner_new();
  gtk_spinner_start(GTK_SPINNER(spinner));

  gtk_box_pack_start(GTK_BOX(content_area), label, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(content_area), spinner, FALSE, FALSE, 5);
  gtk_widget_show_all(dialog);

  rd->dialog = dialog;
  rd->label = label;
  rd->spinner = spinner;
  return;
}

void destroyRunDialog(RunDialog *rd) {
  if (!rd)
    return;
  gtk_widget_destroy(rd->dialog);
  free(rd);
}