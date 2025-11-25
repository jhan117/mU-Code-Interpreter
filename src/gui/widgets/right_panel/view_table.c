#include "gui/gui.h"

#include "core/vm_context.h"
#include "runner.h"

GtkWidget *createRegTable() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  GtkWidget *label = gtk_label_new("CPU Registers");
  GtkListStore *reg_store = gtk_list_store_new(6,
                                               G_TYPE_INT, // cs
                                               G_TYPE_INT, // pc
                                               G_TYPE_INT, // ds
                                               G_TYPE_INT, // ss
                                               G_TYPE_INT, // sp
                                               G_TYPE_INT  // bp
  );
  GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(reg_store));
  GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

  GtkTreeIter iter;
  gtk_list_store_append(reg_store, &iter);
  // 임시 값으로 보여주기
  gtk_list_store_set(reg_store, &iter, // 값 시작
                     0, 0,             // cs
                     1, 0,             // pc
                     2, 0,             // ds
                     3, 0,             // ss
                     4, 0,             // sp
                     5, 0,             // bp
                     -1);

  const char *col_names[] = {
      "CS", "PC", "DS", "SS", "SP", "BP",
  };

  for (int i = 0; i < 6; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "xalign", 0.5, NULL);
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        col_names[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
  }

  gtk_widget_set_halign(label, GTK_ALIGN_START);
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), treeview, TRUE, TRUE, 0);

  getGuiContext()->cur_status_ctx.reg_store = reg_store;
  return box;
}

void updateRegTable(Snapshot *snap) {
  if (!snap)
    return;

  GtkTreeIter iter;
  GtkListStore *store = getGuiContext()->cur_status_ctx.reg_store;

  gtk_list_store_clear(store);
  gtk_list_store_append(store, &iter);

  gtk_list_store_set(store, &iter, 0, snap->cs, 1, snap->pc, 2, snap->ds, 3,
                     snap->ss, 4, snap->sp, 5, snap->bp, -1);
}

void updateLabelsView() {
  VMContext *vm_ctx = getVMContext();
  GuiContext *g_ctx = getGuiContext();
  if (!g_ctx || !g_ctx->labels_store)
    return;

  // 기존 내용 초기화
  gtk_list_store_clear(g_ctx->labels_store);

  for (int i = 0; i < vm_ctx->label_list.count; i++) {
    Label *lbl = &vm_ctx->label_list.labels[i];
    GtkTreeIter iter;
    gtk_list_store_append(g_ctx->labels_store, &iter);
    gtk_list_store_set(g_ctx->labels_store, &iter, 0, lbl->name, 1, lbl->addr,
                       -1);
  }
}

void updateSymbolsView() {
  VMContext *vm_ctx = getVMContext();
  GuiContext *ctx = getGuiContext();
  if (!ctx || !ctx->symbols_store)
    return;
  gtk_list_store_clear(ctx->symbols_store);

  for (int i = 0; i < vm_ctx->symbol_list.count; i++) {
    Symbol *sym = &vm_ctx->symbol_list.symbols[i];
    GtkTreeIter iter;
    gtk_list_store_append(ctx->symbols_store, &iter);
    gtk_list_store_set(ctx->symbols_store, &iter, 0, sym->index, 1, sym->block,
                       2, sym->offset, 3, sym->size, -1);
  }
}

void updateCPUStackView(Snapshot *snap) {
  GuiContext *ctx = getGuiContext();
  if (!ctx || !ctx->cpu_stack_store)
    return;
  gtk_list_store_clear(ctx->cpu_stack_store);

  for (int i = 0; i < snap->cpu_top; i++) {
    GtkTreeIter iter;
    gtk_list_store_prepend(ctx->cpu_stack_store, &iter);
    gtk_list_store_set(ctx->cpu_stack_store, &iter, 0, snap->cpu_stack[i], -1);
  }
}

void updateMemoryView(Snapshot *snap) {
  GuiContext *ctx = getGuiContext();
  if (!ctx || !ctx->memory_store)
    return;

  gtk_list_store_clear(ctx->memory_store);

  // 전체 메모리 출력 (필요 시 범위 조절 가능)
  for (int i = 0; i < INIT_MEMORY_SIZE; i++) {
    GtkTreeIter iter;
    gtk_list_store_prepend(ctx->memory_store, &iter);
    gtk_list_store_set(ctx->memory_store, &iter, 0, snap->memory[i], -1);
  }
}

GtkWidget *createCPUStackTable() {
  GuiContext *ctx = getGuiContext();
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *label = gtk_label_new("CPU Stack");
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

  GtkWidget *treeview;
  GType types[] = {G_TYPE_INT};
  ctx->cpu_stack_store = gtk_list_store_newv(1, types);

  GtkTreeIter iter;
  gtk_list_store_append(ctx->cpu_stack_store, &iter);
  gtk_list_store_set(ctx->cpu_stack_store, &iter, 0, 0, -1);

  treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(ctx->cpu_stack_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);

  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 0.5, NULL);
  GtkTreeViewColumn *column =
      gtk_tree_view_column_new_with_attributes("", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);

  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);
  return box;
}

// Memory Stack (int, 1열, 헤더 없음)
GtkWidget *createMemoryStackTable() {
  GuiContext *ctx = getGuiContext();
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *label = gtk_label_new("Memory");
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

  GtkWidget *treeview;
  GType types[] = {G_TYPE_INT};
  ctx->memory_store = gtk_list_store_newv(1, types);

  GtkTreeIter iter;
  gtk_list_store_append(ctx->memory_store, &iter);
  gtk_list_store_set(ctx->memory_store, &iter, 0, 0, -1);

  treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(ctx->memory_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);

  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 0.5, NULL);
  GtkTreeViewColumn *column =
      gtk_tree_view_column_new_with_attributes("", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);

  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);
  return box;
}

// Labels (Name(str), Addr(int), 2열, 헤더 있음)
GtkWidget *createLabelTable() {
  GuiContext *ctx = getGuiContext();
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *label = gtk_label_new("Labels");
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

  GType types[] = {G_TYPE_STRING, G_TYPE_INT};
  ctx->labels_store = gtk_list_store_newv(2, types);

  GtkTreeIter iter;
  gtk_list_store_append(ctx->labels_store, &iter);
  gtk_list_store_set(ctx->labels_store, &iter, 0, "", 1, 0, -1);

  GtkWidget *treeview =
      gtk_tree_view_new_with_model(GTK_TREE_MODEL(ctx->labels_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);

  GtkCellRenderer *renderer1 = gtk_cell_renderer_text_new();
  g_object_set(renderer1, "xalign", 0.5, NULL);
  GtkTreeViewColumn *col1 = gtk_tree_view_column_new_with_attributes(
      "Name", renderer1, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col1);

  GtkCellRenderer *renderer2 = gtk_cell_renderer_text_new();
  g_object_set(renderer2, "xalign", 0.5, NULL);
  GtkTreeViewColumn *col2 = gtk_tree_view_column_new_with_attributes(
      "Addr", renderer2, "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col2);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);
  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);

  return box;
}

// Symbols (idx(int), block(int), offset(int), size(int), 4열, 헤더 있음)
GtkWidget *createSymbolTable() {
  GuiContext *ctx = getGuiContext();
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  GtkWidget *label = gtk_label_new("Symbols");
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

  GType types[] = {G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT};
  ctx->symbols_store = gtk_list_store_newv(4, types);

  GtkTreeIter iter;
  gtk_list_store_append(ctx->symbols_store, &iter);
  gtk_list_store_set(ctx->symbols_store, &iter, 0, 0, 1, 0, 2, 0, 3, 0, -1);

  GtkWidget *treeview =
      gtk_tree_view_new_with_model(GTK_TREE_MODEL(ctx->symbols_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);

  const char *col_names[] = {"Idx", "Block", "Offset", "Size"};
  for (int i = 0; i < 4; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "xalign", 0.5, NULL);
    GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
        col_names[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
  }

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_widget_set_hexpand(treeview, TRUE);
  gtk_widget_set_vexpand(treeview, TRUE);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);
  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);

  return box;
}

// 결과 통계
GtkWidget *initStatisticsBox() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  // 1. Memory Access Count
  GtkWidget *mem_label = gtk_label_new("Memory Access Count: 0");
  gtk_widget_set_halign(mem_label, GTK_ALIGN_START);
  gtk_box_pack_start(GTK_BOX(box), mem_label, FALSE, FALSE, 0);

  // 2. Instruction Use Count (더미)
  GtkListStore *use_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
  GtkWidget *use_tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(use_store));

  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_append_column(GTK_TREE_VIEW(use_tree),
                              gtk_tree_view_column_new_with_attributes(
                                  "Instruction", renderer, "text", 0, NULL));
  gtk_tree_view_append_column(GTK_TREE_VIEW(use_tree),
                              gtk_tree_view_column_new_with_attributes(
                                  "Use Count", renderer, "text", 1, NULL));

  GtkTreeIter iter;
  // for (int i = 0; i < OPCODE_MAX; i++) {
  //   const OpInfo *op = findOpInfoByOpcode(i);
  //   if (!op)
  //     continue;
  //   gtk_list_store_append(use_store, &iter);
  //   gtk_list_store_set(use_store, &iter, 0, op->name, 1, 0, -1);
  // }

  GtkWidget *use_scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(use_scrolled), use_tree);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(use_scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start(GTK_BOX(box), gtk_label_new("Instruction Use Count"),
                     FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), use_scrolled, TRUE, TRUE, 0);

  // 3. Instruction Run Count (더미)
  GtkListStore *run_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
  GtkWidget *run_tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(run_store));

  gtk_tree_view_append_column(GTK_TREE_VIEW(run_tree),
                              gtk_tree_view_column_new_with_attributes(
                                  "Instruction", renderer, "text", 0, NULL));
  gtk_tree_view_append_column(GTK_TREE_VIEW(run_tree),
                              gtk_tree_view_column_new_with_attributes(
                                  "Run Count", renderer, "text", 1, NULL));

  // for (int i = 0; i < OPCODE_MAX; i++) {
  //   const OpInfo *op = findOpInfoByOpcode(i);
  //   if (!op)
  //     continue;
  //   gtk_list_store_append(run_store, &iter);
  //   gtk_list_store_set(run_store, &iter, 0, op->name, 1, 0, -1); // 0으로
  //   초기화
  // }

  GtkWidget *run_scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(run_scrolled), run_tree);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(run_scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start(GTK_BOX(box), gtk_label_new("Instruction Run Count"),
                     FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), run_scrolled, TRUE, TRUE, 0);

  // GuiContext에 store와 label 저장
  GuiContext *ctx = getGuiContext();
  ctx->stat_mem_label = mem_label;
  ctx->stat_use_store = use_store;
  ctx->stat_run_store = run_store;

  return box;
}

void updateStatisticsBox() {
  GuiContext *ctx = getGuiContext();
  Statistics *stats = &getVMContext()->stat;

  if (stats->memory_access_count > 0) {
    // 메모리 접근
    char buf[64];
    sprintf(buf, "Memory Access Count: %d", stats->memory_access_count);
    gtk_label_set_text(GTK_LABEL(ctx->stat_mem_label), buf);
  }

  // Instruction Use Count
  // 있나 없나 확인
  int has_use = 0;
  for (int i = 0; i < OPCODE_MAX; i++)
    if (stats->inst_use_count[i] > 0) {
      has_use = 1;
      break;
    }
  if (has_use) {
    // gtk_list_store_clear(ctx->stat_use_store);
    for (int i = 0; i < OPCODE_MAX; i++) {
      // if (stats->inst_use_count[i] > 0) {
      //   const OpInfo *op = findOpInfoByOpcode(i);
      //   GtkTreeIter use_iter;
      //   gtk_list_store_append(ctx->stat_use_store, &use_iter);
      //   gtk_list_store_set(ctx->stat_use_store, &use_iter, 0, op->name, 1,
      //                      stats->inst_use_count[i], -1);
      // }
    }
  }

  // Instruction Run Count
  int has_run = 0;
  for (int i = 0; i < OPCODE_MAX; i++)
    if (stats->inst_run_count[i] > 0) {
      has_run = 1;
      break;
    }

  if (has_run) {
    // gtk_list_store_clear(ctx->stat_run_store);
    for (int i = 0; i < OPCODE_MAX; i++) {
      // if (stats->inst_run_count[i] > 0) {
      //   const OpInfo *op = findOpInfoByOpcode(i);
      //   GtkTreeIter run_iter;
      //   gtk_list_store_append(ctx->stat_run_store, &run_iter);
      //   gtk_list_store_set(ctx->stat_run_store, &run_iter, 0, op->name, 1,
      //                      stats->inst_run_count[i], -1);
      // }
    }
  }
}