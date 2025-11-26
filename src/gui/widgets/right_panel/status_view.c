#include "gui/gui_widgets.h"

#include "core/constants.h" // SNAPSHOT_INTERVAL

int find_nearest_snapshot_index(int step) {
  int lower = (step / SNAPSHOT_INTERVAL) * SNAPSHOT_INTERVAL;
  int upper = lower + SNAPSHOT_INTERVAL;

  if (step - lower < upper - step)
    return lower / SNAPSHOT_INTERVAL; // index
  else
    return upper / SNAPSHOT_INTERVAL; // index
}

GtkWidget *initStatusView() {
  GtkWidget *status_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

  gtk_widget_set_margin_top(status_box, 4);
  gtk_widget_set_margin_bottom(status_box, 4);
  gtk_widget_set_margin_start(status_box, 8);
  gtk_widget_set_margin_end(status_box, 8);

  GtkWidget *cpu = initCPUView();
  GtkWidget *mem = initMemoryView();
  GtkWidget *labels = initLabelsView();
  GtkWidget *symbols = initSymbolsView();

  // 구분 선
  GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  GtkWidget *sep3 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);

  gtk_box_pack_start(GTK_BOX(status_box), cpu, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), sep1, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(status_box), mem, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), sep2, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(status_box), labels, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), sep3, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(status_box), symbols, TRUE, TRUE, 0);

  StatusContext status_ctx = getGuiContext()->status_ctx;
  status_ctx.status_box = status_box;
  status_ctx.cpu_view = cpu;
  status_ctx.memory_view = mem;
  status_ctx.labels_view = status_box;
  status_ctx.symbols_view = symbols;
  status_ctx.sep1 = sep1;
  status_ctx.sep2 = sep2;
  status_ctx.sep3 = sep3;

  return status_box;
}

void updateStatusView(int step) {
  // updateRegView();
  // updateCPUView();
  // updateMemoryView();

  // SnapshotList *snap_list = getSnapshotList();
  // int snap_index = find_nearest_snapshot_index(step);

  // if (snap_index >= SNAPSHOT_COUNT)
  //   snap_index = SNAPSHOT_COUNT - 1;

  // Snapshot snap = snap_list->snapshot_list[snap_index]; // 기준 snapshot 복사

  // int steps_since_snapshot = step - snap_index * SNAPSHOT_INTERVAL;
  // Change *head = NULL;

  // int applied = 0;

  // printf("snap_idx = %d %d\n", snap_index, steps_since_snapshot);

  // while (head && applied < steps_since_snapshot) {
  //   switch (head->hdware_num) {
  //   case HD_NUM_MEMORY:
  //     if (snap.memory[head->addr] != head->new_value) {
  //       snap.memory[head->addr] = head->new_value;
  //     }
  //     break;
  //   case HD_NUM_CPU_STACK:
  //     if (snap.cpu_stack[head->addr] != head->new_value) {
  //       snap.cpu_stack[head->addr] = head->new_value;
  //     }
  //     break;
  //   case HD_NUM_CS:
  //     if (snap.cs != head->new_value) {
  //       printf("%d, %d\n", snap.cs, head->new_value);
  //       snap.cs = head->new_value;
  //     }
  //     break;
  //   case HD_NUM_PC:
  //     if (snap.pc != head->new_value) {
  //       snap.pc = head->new_value;
  //     }
  //     break;
  //   case HD_NUM_DS:
  //     if (snap.ds != head->new_value) {
  //       snap.ds = head->new_value;
  //     }
  //     break;
  //   case HD_NUM_SS:
  //     if (snap.ss != head->new_value) {
  //       snap.ss = head->new_value;
  //     }
  //     break;
  //   case HD_NUM_SP:
  //     if (snap.sp != head->new_value) {
  //       snap.sp = head->new_value;
  //     }
  //     break;
  //   case HD_NUM_BP:
  //     if (snap.bp != head->new_value) {
  //       snap.bp = head->new_value;
  //     }
  //     break;
  //   }
  //   head = head->next;
  //   applied++;
  // }

  // updateRegTable(&snap);
  // updateCPUStackView(&snap);
  // updateMemoryView(&snap);
}