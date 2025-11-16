#include "gui.h"

GtkWidget *createFileChooser() {
  GtkWidget *button =
      gtk_file_chooser_button_new("파일 선택", GTK_FILE_CHOOSER_ACTION_OPEN);

  return button;
}