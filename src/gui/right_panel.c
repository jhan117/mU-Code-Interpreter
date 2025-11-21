#include "gui/gui.h"

GtkWidget *createRightPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  // cpu register view
  GtkWidget *reg = createRegTable();
  gtk_widget_set_halign(reg, GTK_ALIGN_START);

  GtkWidget *status_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

  // 그 외 view
  GtkWidget *cpu = createViewTable("CPU Stack");
  GtkWidget *mem = createViewTable("Memory Stack");
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

  // 입출력 view
  GtkWidget *io_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *io_label = gtk_label_new("입출력");
  gtk_widget_set_halign(io_label, GTK_ALIGN_START);
  GtkWidget *io_view = gtk_text_view_new();

  gtk_box_pack_start(GTK_BOX(io_box), io_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(io_box), io_view, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(box), io_box, TRUE, TRUE, 0);
  return box;
}