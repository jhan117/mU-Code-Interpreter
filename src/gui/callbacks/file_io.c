#include "gui/gui_callbacks.h"

void onFileChosen(GtkFileChooserButton *chooser) {
  FileContext *file_ctx = &getGuiContext()->file_ctx;

  char *filename = gtk_file_chooser_get_filename(chooser);
  if (!filename)
    return;

  g_free(file_ctx->uco_filename);
  file_ctx->uco_filename = g_strdup(filename);

  char **lines = NULL;
  int line_count = 0;
  loadUco(filename, &lines, &line_count);
  updateUcodeView(&lines, &line_count);

  g_free(filename);
}