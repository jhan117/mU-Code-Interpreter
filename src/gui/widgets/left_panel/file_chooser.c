#include "gui/gui.h"

void onFileChosen(GtkFileChooserButton *chooser);

GtkWidget *createFileChooser() {
  GtkWidget *button =
      gtk_file_chooser_button_new("파일 선택", GTK_FILE_CHOOSER_ACTION_OPEN);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "uCode 파일 (*.uco)");
  gtk_file_filter_add_pattern(filter, "*.uco");

  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(button), filter);

  return button;
}

void onFileChosen(GtkFileChooserButton *chooser) {
  GuiContext *ctx = getGuiContext();

  char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
  if (!filename)
    return;

  g_free(ctx->uco_filename);
  ctx->uco_filename = g_strdup(filename);
  loadUcoToTable(filename);
  g_free(filename);
}