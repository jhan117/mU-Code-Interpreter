#include "gui.h"

GtkWidget *createRightPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  GtkWidget *reg = createViewTable("CPU Registers");
  gtk_widget_set_halign(reg,
                        GTK_ALIGN_START); // 나중에 1행으로 고정하는 기능 추가

  GtkWidget *status_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

  GtkWidget *cpu = createViewTable("CPU Stack");
  GtkWidget *mem = createViewTable("Memory");
  GtkWidget *labels = createViewTable("Labels");
  GtkWidget *symbols = createViewTable("Symbols");

  gtk_box_pack_start(GTK_BOX(status_box), cpu, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), mem, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), labels, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), symbols, TRUE, TRUE, 0);

  NotebookPageInfo pages_mid[] = {
      {"현재 상태", status_box},
      {"결과 통계", gtk_text_view_new()},
  };

  GtkWidget *notebook_mid = createNotebookView(pages_mid, 2);

  addRow("첫 번째 데이터");
  addRow("두 번째 데이터");
  addRow("세 번째 데이터");

  gtk_box_pack_start(GTK_BOX(box), reg, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), notebook_mid, FALSE, FALSE, 0);

  GtkWidget *log_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *log_label = gtk_label_new("입출력");
  gtk_widget_set_halign(log_label, GTK_ALIGN_START);
  GtkWidget *log_view = gtk_text_view_new();

  gtk_box_pack_start(GTK_BOX(log_box), log_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(log_box), log_view, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(box), log_box, TRUE, TRUE, 0);
  return box;
}