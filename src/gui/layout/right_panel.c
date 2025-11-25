#include "gui/gui.h"

#include "gui/gui_widgets.h"

GtkWidget *createRightPanel() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

  // cpu register view
  GtkWidget *reg = createRegTable();
  gtk_widget_set_halign(reg, GTK_ALIGN_START);

  GtkWidget *status_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

  // 그 외 view
  GtkWidget *cpu = createCPUStackTable();
  GtkWidget *mem = createMemoryStackTable();
  GtkWidget *labels = createLabelTable();
  GtkWidget *symbols = createSymbolTable();

  gtk_box_pack_start(GTK_BOX(status_box), cpu, TRUE, TRUE, 0);
  GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(status_box), sep1, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(status_box), mem, TRUE, TRUE, 0);
  GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(status_box), sep2, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(status_box), labels, TRUE, TRUE, 0);
  GtkWidget *sep3 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(status_box), sep3, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(status_box), symbols, TRUE, TRUE, 0);
  gtk_widget_set_margin_top(status_box, 4);
  gtk_widget_set_margin_bottom(status_box, 4);
  gtk_widget_set_margin_start(status_box, 8);
  gtk_widget_set_margin_end(status_box, 8);

  NotebookPageInfo pages_mid[] = {
      {"현재 상태", status_box},
      {"결과 통계", initStatisticsBox()},
  };

  GtkWidget *notebook_mid = createNotebookView(pages_mid, 2);

  gtk_box_pack_start(GTK_BOX(box), reg, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), notebook_mid, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), initIOView(), TRUE, TRUE, 0);

  return box;
}