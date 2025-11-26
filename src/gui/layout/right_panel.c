#include "gui/gui.h"

#include "gui/gui_widgets.h"

GtkWidget *createRightPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  // 상단 reg view
  GtkWidget *reg = createRegView();

  // 탭: 현재 상태 + 결과 통계 (둘 다 편집 불가)
  NotebookPageInfo pages_mid[] = {
      {"현재 상태", createStatusView()},
      {"결과 통계", createStatisticsView()},
  };

  gtk_box_pack_start(GTK_BOX(box), reg, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), createNotebookTab(pages_mid, 2), TRUE, TRUE,
                     0);
  gtk_box_pack_start(GTK_BOX(box), createIOView(), TRUE, TRUE, 0);

  getGuiContext()->status_ctx.reg_view = reg;

  return box;
}