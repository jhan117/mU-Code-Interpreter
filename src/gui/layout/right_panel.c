#include "gui/gui.h"

#include "gui/gui_widgets.h"

GtkWidget *createRightPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  // 상단 reg view
  GtkWidget *reg = createRegView();

  TextScrollInfo io_info = createIOView();
  GtkWidget *status_box = createStatusView();
  TextScrollInfo lst_info = createLstView();

  // 탭: 현재 상태 + 결과 통계 (둘 다 편집 불가)
  NotebookPageInfo pages_mid[] = {
      {"입출력", io_info.scroll},
      {"결과 통계", createStatisticsView()},
      {"lst 파일 출력", lst_info.scroll},
  };

  gtk_box_pack_start(GTK_BOX(box), reg, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), status_box, TRUE, TRUE, 8);
  gtk_box_pack_start(GTK_BOX(box), createNotebookTab(pages_mid, 3), TRUE, TRUE,
                     0);

  GuiContext *ctx = getGuiContext();
  ctx->status_ctx.reg_view = reg;
  ctx->io_ctx.io_view = io_info.text_view;
  ctx->lst_view = lst_info.text_view;
  ctx->status_ctx.status_box = status_box;
  return box;
}