#include "gui/gui_callbacks.h"

#include "assembler/assemble.h" // assemble()
#include "runner/runner.h"      // runner()

static gboolean finish(gpointer data) {
  WorkerData *wd = (WorkerData *)data;
  free(wd);

  toggleWidgetsVisible(1);

  IOContext io_ctx = getGuiContext()->io_ctx;
  gtk_text_view_set_editable(GTK_TEXT_VIEW(io_ctx.io_view), FALSE);

  insertAtEnd(io_ctx.io_view, "\n=== 프로그램 종료 ===\n");

  return G_SOURCE_REMOVE;
}

static gboolean updateUIAfterRun(gpointer data) {
  updateAssembleView();
  updateLabelsView();
  updateSymbolsView();
  updateStatisticsView();
  updateStatusView();
  updateLstView();
  initStep();
  return G_SOURCE_REMOVE;
}

static gboolean showErrorMessage(gpointer data) {
  ErrorResult *err = (ErrorResult *)data;

  if (err->src == ERR_SRC_ASSEMBLE)
    showMessage(GTK_MESSAGE_ERROR, formatAsmError(err->code, err->line));
  else
    showMessage(GTK_MESSAGE_ERROR, formatRunError(err->line));

  g_free(err);

  return G_SOURCE_REMOVE;
}

static gpointer runWorkerThread(gpointer arg) {
  WorkerData *wd = (WorkerData *)arg;

  ErrorResult asm_res = assemble(wd->lines, wd->line_count);
  freeUcoView(wd->lines, wd->line_count);

  if (asm_res.code != 0) {
    ErrorResult *err = g_new0(ErrorResult, 1);
    *err = asm_res;
    g_idle_add(showErrorMessage, err);
    g_idle_add(finish, wd);
    return NULL;
  }

  ErrorResult run_res = runner();
  if (run_res.code != 0) {
    ErrorResult *err = g_new0(ErrorResult, 1);
    *err = run_res;
    g_idle_add(showErrorMessage, err);
    g_idle_add(finish, wd);
    return NULL;
  }

  g_idle_add(updateUIAfterRun, NULL);
  g_idle_add(finish, wd);

  return NULL;
}

void startWorker(WorkerData *wd) {
  g_thread_new("worker", runWorkerThread, wd);
}