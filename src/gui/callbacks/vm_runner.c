#include "gui/gui_callbacks.h"

#include "assembler/assemble.h" // assemble()
#include "io_utils/io_utils.h"  // freeUco()

void onRun(GtkButton *button) {
  initVMContext();
  GuiContext *ctx = getGuiContext();
  ctx->is_run_done = 0;

  initRunDialog(&ctx->run_dialog, ctx->main_window, "실행 상태",
                "어셈블러 중...");

  char **lines = NULL;
  int line_count = 0;
  if (!getUcodeView(&lines, &line_count)) {
    gtk_label_set_text(GTK_LABEL(ctx->run_dialog.label), "어셈블러 실패");
    gtk_dialog_run(GTK_DIALOG(ctx->run_dialog.dialog));
    destroyRunDialog(ctx->run_dialog);
    return;
  }

  int asm_res = assemble(lines, line_count);
  freeUco(lines, line_count);
  if (asm_res != ASSEMBLE_ERR_NONE) {
    gtk_label_set_text(GTK_LABEL(ctx->run_dialog.label), "어셈블러 오류");
    gtk_dialog_run(GTK_DIALOG(ctx->run_dialog.dialog));
    destroyRunDialog(ctx->run_dialog);
    return;
  }

  updateAssembleView();
  initStep();
  gtk_label_set_text(GTK_LABEL(ctx->run_dialog.label), "어셈블러 완료");

  gtk_label_set_text(GTK_LABEL(ctx->run_dialog.label), "실행 중...");
  int run_res = runner();
  if (run_res != 0) {
    gtk_label_set_text(GTK_LABEL(ctx->run_dialog.label), "실행 오류");
    gtk_dialog_run(GTK_DIALOG(ctx->run_dialog.dialog));
    destroyRunDialog(ctx->run_dialog);
    return;
  }

  updateLabelsView();
  updateSymbolsView();
  updateStatisticsView();
  // updateStatus();

  gtk_spinner_stop(GTK_SPINNER(ctx->run_dialog.spinner));
  gtk_label_set_text(GTK_LABEL(ctx->run_dialog.label), "실행 완료");

  ctx->is_run_done = 1;
  ctx->step_ctx.current_step = 0;

  gtk_dialog_run(GTK_DIALOG(ctx->run_dialog.dialog));
  destroyRunDialog(ctx->run_dialog);
}