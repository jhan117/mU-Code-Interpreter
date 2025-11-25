#include "gui/gui.h"

GtkWidget *createLeftPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  // 파일 선택 버튼
  GtkWidget *chooserBtn = initFileChooser();
  g_signal_connect(chooserBtn, "file-set", G_CALLBACK(onFileChosen), NULL);

  // 탭: ucode 에디터 + 어셈블 뷰(편집 불가)
  GtkWidget *ucode_scroll = initUcodeView();
  TextScrollInfo assemble_info = initAssembleView();

  NotebookPageInfo pages[] = {
      {"uCode", ucode_scroll},
      {"assemble 결과", assemble_info.scroll},
  };

  // 실행 버튼
  GtkWidget *run_btn = gtk_button_new_with_label("Run");
  gtk_widget_set_halign(run_btn, GTK_ALIGN_CENTER);

  // 실행 이벤트
  g_signal_connect(run_btn, "clicked", G_CALLBACK(onRun), NULL);

  gtk_box_pack_start(GTK_BOX(box), chooserBtn, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookView(pages, 2), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), initStepControl(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), run_btn, FALSE, FALSE, 0);

  GuiContext *ctx = getGuiContext();
  ctx->file_ctx.file_chooser = chooserBtn;
  ctx->code_ctx.assemble_view = assemble_info;
  ctx->run_btn = run_btn;
  ctx->is_run_done = 0;

  return box;
}