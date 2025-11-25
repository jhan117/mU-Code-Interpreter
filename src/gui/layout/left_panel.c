#include "gui/gui.h"

#include "gui/gui_callbacks.h"
#include "gui/gui_widgets.h"

GtkWidget *createLeftPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  // 파일 선택 버튼
  GtkWidget *chooserBtn = initFileChooser();
  g_signal_connect(chooserBtn, "file-set", G_CALLBACK(onFileChosen), NULL);

  // 탭: ucode 에디터 + 어셈블 뷰(편집 불가)
  GtkWidget *ucode_scroll = initUcodeView();
  TextScrollInfo assemble_info = initAssembleView();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(assemble_info.text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(assemble_info.text_view),
                                   FALSE);

  NotebookPageInfo pages[] = {
      {"uCode", ucode_scroll},
      {"assemble 결과", assemble_info.scroll},
  };

  // 실행 버튼
  GtkWidget *run_btn = gtk_button_new_with_label("Run");
  gtk_widget_set_halign(run_btn, GTK_ALIGN_CENTER);
  g_signal_connect(run_btn, "clicked", G_CALLBACK(onRun), NULL);

  gtk_box_pack_start(GTK_BOX(box), chooserBtn, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookView(pages, 2), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), createStepControl(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), run_btn, FALSE, FALSE, 0);

  GuiContext *ctx = getGuiContext();
  ctx->file_ctx.file_chooser = chooserBtn;
  ctx->code_ctx.assemble_view = assemble_info;
  ctx->is_run_done = 0;

  return box;
}