#include "gui/gui.h"

GtkWidget *createLeftPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  // 파일 선택 버튼
  GtkWidget *chooserBtn = createFileChooser();
  g_signal_connect(chooserBtn, "file-set", G_CALLBACK(onFileChosen), NULL);

  // 탭: ucode 에디터 + 어셈블 뷰(편집 불가)
  TextScrollInfo ucode_info = createUcodeView();
  TextScrollInfo assemble_info = createAssembleView();

  NotebookPageInfo pages[] = {
      {"uCode", ucode_info.scroll},
      {"assemble 결과", assemble_info.scroll},
  };

  // 실행 버튼
  GtkWidget *run_btn = gtk_button_new_with_label("Run");
  gtk_widget_set_halign(run_btn, GTK_ALIGN_CENTER);

  // 실행 이벤트
  g_signal_connect(run_btn, "clicked", G_CALLBACK(onRun), NULL);

  gtk_box_pack_start(GTK_BOX(box), chooserBtn, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookTab(pages, 2), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), createStepControl(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), run_btn, FALSE, FALSE, 0);

  GuiContext *ctx = getGuiContext();
  ctx->file_ctx.file_chooser = chooserBtn;
  ctx->code_ctx.ucode_view = ucode_info;
  ctx->code_ctx.assemble_view = assemble_info;
  ctx->run_btn = run_btn;

  return box;
}