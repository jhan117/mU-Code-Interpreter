#include "gui/gui_callbacks.h"

#include "assembler/assemble.h" // assemble()
#include "io_utils/io_utils.h"  // freeUco()

static gboolean finish(gpointer data) {
  WorkerData *wd = (WorkerData *)data;
  if (wd) {
    free(wd);
  }
  toggleWidgetsVisible(1);
  return FALSE;
}

static gboolean updateStatus(gpointer data) {
  updateLabelsView();
  updateSymbolsView();
  updateStatisticsView();
  return FALSE;
}

static gboolean updateAssemble(gpointer data) {
  updateAssembleView();
  return FALSE;
}

static gboolean markRunDone(gpointer data) {
  GuiContext *ctx = getGuiContext();
  ctx->is_run_done = 1;
  ctx->step_ctx.current_step = 0;
  return FALSE;
}

static gpointer runWorkerThread(gpointer arg) {
  WorkerData *wd = (WorkerData *)arg;

  if (wd->line_count <= 0) {
    g_idle_add(finish, wd);
    return NULL;
  }

  int asm_res = assemble(wd->lines, wd->line_count);
  freeUco(wd->lines, wd->line_count);

  if (asm_res != ASSEMBLE_ERR_NONE) {
    g_idle_add(finish, wd);
    return NULL;
  }

  g_idle_add(updateAssemble, NULL);

  int run_res = runner();
  if (run_res != 0) {
    g_idle_add(finish, wd);
    return NULL;
  }

  g_idle_add(updateStatus, NULL);
  g_idle_add(markRunDone, NULL);
  g_idle_add(finish, wd);

  return NULL;
}

void startWorker(WorkerData *wd) {
  g_thread_new("worker", runWorkerThread, wd);
}