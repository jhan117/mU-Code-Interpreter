#include "gui/gui_widgets.h"

GtkWidget *createFileChooser() {
  GtkWidget *button =
      gtk_file_chooser_button_new("파일 선택", GTK_FILE_CHOOSER_ACTION_OPEN);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "uCode 파일 (*.uco)");
  gtk_file_filter_add_pattern(filter, "*.uco");

  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(button), filter);

  return button;
}
