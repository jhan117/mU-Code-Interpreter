#include "gui/callbacks.h"

#include "gui/gui_context.h"

void onFileChosen(GtkFileChooserButton *chooser) {
  FileContext *file_ctx = &getGuiContext()->file_ctx;

  char *filename = gtk_file_chooser_get_filename(chooser);
  if (!filename)
    return;

  g_free(file_ctx->uco_filename);
  file_ctx->uco_filename = g_strdup(filename);
  loadUcoToTable(filename);
  g_free(filename);
}