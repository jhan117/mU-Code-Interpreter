#include "gui/gui_callbacks.h"

#include "core/vm_context.h"
#include <stdlib.h>

void onRun(GtkButton *button) {
  GuiContext *ctx = getGuiContext();
  if (ctx->is_run) {
    showMessage(GTK_MESSAGE_WARNING, "이미 실행 중입니다.");
    return;
  }
  initVMContext();
  getVMContext()->run_mode = GUI;
  toggleWidgetsVisible(0);
  ctx->is_run = 1;
  IOContext *io_ctx = &ctx->io_ctx;

  gtk_text_view_set_editable(GTK_TEXT_VIEW(io_ctx->io_view), TRUE);
  highlightLine(ctx->code_ctx.ucode_view.text_view, -1);
  highlightLine(ctx->code_ctx.assemble_view.text_view, -1);
  io_ctx->is_last_write = 0;
  io_ctx->input_queue = g_async_queue_new();

  char **lines = NULL;
  int line_count = 0;
  if (!getUcodeView(&lines, &line_count)) {
    freeUcoView(lines, line_count);
    return;
  }

  if (line_count == 0) {
    freeUcoView(lines, line_count);
    return;
  }

  WorkerData *wd = (WorkerData *)calloc(1, sizeof(WorkerData));
  if (!wd) {
    freeUcoView(lines, line_count);
    return;
  }

  wd->lines = lines;
  wd->line_count = line_count;

  io_ctx->line_count = wd->line_count;
  io_ctx->lines = malloc(sizeof(char *) * wd->line_count);
  for (int i = 0; i < wd->line_count; i++)
    io_ctx->lines[i] = g_strdup(wd->lines[i]);

  insertAtEnd(io_ctx->io_view, "=== 프로그램 시작 ===\n");
  insertAtEnd(io_ctx->io_view, ">> ");

  startWorker(wd);
}