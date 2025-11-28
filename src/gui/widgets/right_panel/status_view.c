#include "gui/gui_widgets.h"

#include "core/vm_context.h"
#include "record/record.h" // restoreSnapshot(), applyChanges()

GtkWidget *createStatusView() {
  GtkWidget *status_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

  gtk_widget_set_margin_top(status_box, 4);
  gtk_widget_set_margin_bottom(status_box, 4);
  gtk_widget_set_margin_start(status_box, 8);
  gtk_widget_set_margin_end(status_box, 8);

  GtkWidget *cpu = createCPUView();
  GtkWidget *mem = createMemoryView();
  GtkWidget *labels = createLabelsView();
  GtkWidget *symbols = createSymbolsView();

  gtk_box_pack_start(GTK_BOX(status_box), cpu, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), mem, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), labels, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(status_box), symbols, TRUE, TRUE, 0);

  StatusContext *status_ctx = &getGuiContext()->status_ctx;
  status_ctx->cpu_view = cpu;
  status_ctx->memory_view = mem;
  status_ctx->labels_view = labels;
  status_ctx->symbols_view = symbols;

  return status_box;
}

void updateStatusView() {
  VMContext *ctx = getVMContext();
  StepContext *step_ctx = &getGuiContext()->step_ctx;

  int target_step = gtk_adjustment_get_value(step_ctx->adj);
  SnapshotList *snapshot_list = &ctx->snapshot_list;

  int snap_idx = target_step / SNAPSHOT_INTERVAL;
  int snapshot_count = snapshot_list->snapshot_count;
  if (snapshot_count == 0) {
    return;
  }

  if (snap_idx >= snapshot_count)
    snap_idx = snapshot_count - 1;

  Snapshot *snap = &snapshot_list->snapshot_list[snap_idx];
  int snap_step = snap_idx * SNAPSHOT_INTERVAL;

  restoreSnapshot(snap);

  if (snap_step < target_step) {
    applyChanges(snap_step + 1, target_step);
  }

  updateRegView();
  updateCPUView();
  updateMemoryView();
}