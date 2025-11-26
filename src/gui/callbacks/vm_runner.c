#include "gui/gui_callbacks.h"

#include "core/vm_context.h"
#include <stdlib.h>

void onRun(GtkButton *button) {
  initVMContext();
  GuiContext *ctx = getGuiContext();
  getVMContext()->run_mode = GUI;
  toggleWidgetsVisible(0);
  ctx->is_run_done = 0;
  ctx->io_ctx.input_queue = g_async_queue_new();

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

  startWorker(wd);
}