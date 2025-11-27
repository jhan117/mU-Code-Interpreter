#include "gui/gui_callbacks.h"

#include "core/vm_context.h"
#include <stdlib.h>

void onRun(GtkButton *button) {
  initVMContext();
  GuiContext *ctx = getGuiContext();
  getVMContext()->run_mode = GUI;
  toggleWidgetsVisible(0);
  IOContext *io_ctx = &ctx->io_ctx;
  io_ctx->input_queue = g_async_queue_new();

  char **lines = NULL;
  int line_count = 0;
  if (!getUcodeView(&lines, &line_count)) {
    freeUco(lines, line_count);
    return;
  }

  WorkerData *wd = (WorkerData *)calloc(1, sizeof(WorkerData));
  if (!wd) {
    freeUco(lines, line_count);
    return;
  }

  wd->lines = lines;
  wd->line_count = line_count;

  io_ctx->line_count = wd->line_count;
  io_ctx->lines = malloc(sizeof(char *) * wd->line_count);
  for (int i = 0; i < wd->line_count; i++)
    io_ctx->lines[i] = g_strdup(wd->lines[i]);

  startWorker(wd);
}