#include "gui/gui_widgets.h"

#include "core/instruction.h" // getOpInfo()
#include "core/vm_context.h"

GtkWidget *createStatisticsView() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  // === 메모리 접근 횟수 === //
  GtkWidget *mem_label = gtk_label_new("Memory Access Count: 0");
  gtk_widget_set_halign(mem_label, GTK_ALIGN_START);

  // === 명령어 사용 횟수 === //
  GtkWidget *use_label = gtk_label_new("Instruction Use Count");
  gtk_widget_set_halign(use_label, GTK_ALIGN_START);

  GtkListStore *use_store =
      gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING,
                         G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
  GtkWidget *use_tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(use_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(use_tree), FALSE);
  gtk_tree_view_set_grid_lines(use_tree, GTK_TREE_VIEW_GRID_LINES_BOTH);

  GtkTreeSelection *use_selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(use_tree));
  gtk_tree_selection_set_mode(use_selection, GTK_SELECTION_NONE);

  int op_count;
  const OpInfo *op = getOpInfo(&op_count);
  GtkTreeIter iter;
  for (int i = 4; i < op_count; i += 3) {
    gtk_list_store_append(use_store, &iter);
    gtk_list_store_set(use_store, &iter, 0, op[i].name, 1, 0, 2,
                       (i + 1 < op_count ? op[i + 1].name : ""), 3, 0, 4,
                       (i + 2 < op_count ? op[i + 2].name : ""), 5, 0, -1);
  }

  for (int i = 0; i < 6; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "xalign", 0.5, NULL);
    GtkTreeViewColumn *column =
        gtk_tree_view_column_new_with_attributes("", renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(use_tree), column);
  }

  GtkWidget *use_scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(use_scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(use_scrolled), use_tree);

  // === 명령어 실행 횟수 === //
  GtkWidget *run_label = gtk_label_new("Instruction Run Count");
  gtk_widget_set_halign(run_label, GTK_ALIGN_START);

  GtkListStore *run_store =
      gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING,
                         G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
  GtkWidget *run_tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(run_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(run_tree), FALSE);
  gtk_tree_view_set_grid_lines(run_tree, GTK_TREE_VIEW_GRID_LINES_BOTH);

  GtkTreeSelection *run_selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(run_tree));
  gtk_tree_selection_set_mode(run_selection, GTK_SELECTION_NONE);

  for (int i = 4; i < op_count; i += 3) {
    gtk_list_store_append(run_store, &iter);
    gtk_list_store_set(run_store, &iter, 0, op[i].name, 1, 0, 2,
                       (i + 1 < op_count ? op[i + 1].name : ""), 3, 0, 4,
                       (i + 2 < op_count ? op[i + 2].name : ""), 5, 0, -1);
  }

  for (int i = 0; i < 6; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "xalign", 0.5, NULL);
    GtkTreeViewColumn *column =
        gtk_tree_view_column_new_with_attributes("", renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(run_tree), column);
  }

  GtkWidget *run_scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(run_scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(run_scrolled), run_tree);

  GtkWidget *count_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  GtkWidget *use_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *run_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  gtk_box_pack_start(GTK_BOX(box), mem_label, FALSE, FALSE, 8);
  gtk_box_pack_start(GTK_BOX(use_box), use_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(use_box), use_scrolled, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(run_box), run_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(run_box), run_scrolled, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(count_box), use_box, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(count_box), run_box, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(box), count_box, TRUE, TRUE, 8);

  gtk_widget_set_margin_left(box, 8);
  gtk_widget_set_margin_right(box, 8);

  StatContext *stat_ctx = &getGuiContext()->stat_ctx;
  stat_ctx->stat_mem_label = mem_label;
  stat_ctx->stat_use_store = use_store;
  stat_ctx->stat_run_store = run_store;

  return box;
}

void updateStatisticsView() {
  Statistics stat = getVMContext()->stat;
  StatContext stat_ctx = getGuiContext()->stat_ctx;

  // === 메모리 접근 횟수 === //
  if (stat.memory_access_count > 0) {
    char buf[64];
    sprintf(buf, "Memory Access Count: %d", stat.memory_access_count);
    gtk_label_set_text(GTK_LABEL(stat_ctx.stat_mem_label), buf);
  }

  // === 명령어 사용 횟수 === //
  GtkListStore *use_store = stat_ctx.stat_use_store;
  gtk_list_store_clear(use_store);

  int op_count;
  const OpInfo *op = getOpInfo(&op_count);
  GtkTreeIter iter;
  for (int i = 4; i < op_count; i += 3) {
    const OpInfo *a = &op[i];
    const OpInfo *b = (i + 1 < op_count) ? &op[i + 1] : NULL;
    const OpInfo *c = (i + 2 < op_count) ? &op[i + 2] : NULL;

    gtk_list_store_append(use_store, &iter);

    gtk_list_store_set(
        use_store, &iter, 0, a->name, 1, stat.inst_use_count[a->opcode], 2,
        b ? b->name : "", 3, b ? stat.inst_use_count[b->opcode] : 0, 4,
        c ? c->name : "", 5, c ? stat.inst_use_count[c->opcode] : 0, -1);
  }

  // === 명령어 실행 횟수 === //
  GtkListStore *run_store = stat_ctx.stat_run_store;
  gtk_list_store_clear(run_store);

  for (int i = 4; i < op_count; i += 3) {
    const OpInfo *a = &op[i];
    const OpInfo *b = (i + 1 < op_count) ? &op[i + 1] : NULL;
    const OpInfo *c = (i + 2 < op_count) ? &op[i + 2] : NULL;

    gtk_list_store_append(run_store, &iter);

    gtk_list_store_set(
        run_store, &iter, 0, a->name, 1, stat.inst_run_count[a->opcode], 2,
        b ? b->name : "", 3, b ? stat.inst_run_count[b->opcode] : 0, 4,
        c ? c->name : "", 5, c ? stat.inst_run_count[c->opcode] : 0, -1);
  }
}