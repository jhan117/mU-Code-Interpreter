#include "gui/gui_widgets.h"

#include "core/vm_context.h"

GtkWidget *createUcodeView() {
  GtkListStore *store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,
                                           G_TYPE_STRING, G_TYPE_STRING);

  GtkWidget *tree_view = gtk_tree_view_new_with_model(store);
  gtk_tree_view_set_grid_lines(tree_view, GTK_TREE_VIEW_GRID_LINES_BOTH);

  GtkCellRenderer *renderer1 =
      addColumn(tree_view, store, "Label", 0, onLabelEdited);
  GtkCellRenderer *renderer2 =
      addColumn(tree_view, store, "Operator", 1, onOperatorEdited);
  GtkCellRenderer *renderer3 =
      addColumn(tree_view, store, "Operand", 2, onOperandEdited);

  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "", 1, "", 2, "", -1);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled), tree_view);
  gtk_scrolled_window_set_policy(scrolled, GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  // 행 추가/삭제 키 입력 이벤트 (중간 삽입 가능하도록 나중에 추가하자...)
  g_signal_connect(tree_view, "key-press-event", G_CALLBACK(onKeyPress), NULL);

  TableInfo *ucode_table = &getGuiContext()->code_ctx.ucode_table;
  ucode_table->tree_view = tree_view;
  ucode_table->list_data = store;
  ucode_table->renderer[0] = renderer1;
  ucode_table->renderer[1] = renderer2;
  ucode_table->renderer[2] = renderer3;

  return scrolled;
}

void updateUcodeView(char ***lines, int *line_count) {
  GuiContext *ctx = getGuiContext();

  GtkListStore *store = ctx->code_ctx.ucode_table.list_data;
  gtk_list_store_clear(store);

  for (int i = 0; i < *line_count; i++) {
    char label[MAX_LABEL_LEN] = "";
    char opcode[MAX_OP_LEN] = "";
    char operands[LINE_BUFFER_LEN] = "";

    if (!parseTable((*lines)[i], label, opcode, operands)) {
      printf("gui parsing 오류!\n");
      return;
    }

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, label, 1, opcode, 2, operands, -1);
  }

  freeUco(*lines, *line_count);
  ctx->step_ctx.current_step = 0;
}

int getUcodeView(char ***lines, int *line_count) {
  GtkTreeModel *model =
      GTK_TREE_MODEL(getGuiContext()->code_ctx.ucode_table.list_data);

  GtkTreeIter iter;
  if (!gtk_tree_model_get_iter_first(model, &iter))
    return 0;

  int capacity = INIT_LINE_CAPACITY;
  char **result = malloc(sizeof(char *) * capacity);
  if (!result)
    return 0;
  *line_count = 0;

  do {
    char *label = NULL;
    char *op = NULL;
    char *oper = NULL;
    gtk_tree_model_get(model, &iter, 0, &label, 1, &op, 2, &oper, -1);

    if ((!label || label[0] == '\0') && (!op || op[0] == '\0') &&
        (!oper || oper[0] == '\0')) {
      g_free(label);
      g_free(op);
      g_free(oper);
      continue;
    }

    char *line = g_strdup_printf("%-10.10s %s %s\n", label ? label : "",
                                 op ? op : "", oper ? oper : "");

    g_free(label);
    g_free(op);
    g_free(oper);

    if (!line) {
      for (int i = 0; i < *line_count; i++)
        free(result[i]);
      free(result);
      return 0;
    }

    if (*line_count >= capacity) {
      capacity *= 2;
      char **tmp = realloc(result, sizeof(char *) * capacity);
      if (!tmp) {
        free(line);
        for (int i = 0; i < *line_count; i++)
          free(result[i]);
        free(result);
        return 0;
      }
      result = tmp;
    }

    result[*line_count] = line;
    (*line_count)++;

  } while (gtk_tree_model_iter_next(model, &iter));

  *lines = result;
  return 1;
}

void highlightRow() {
  GuiContext *ctx = getGuiContext();
  GtkListStore *store = ctx->code_ctx.ucode_table.list_data;
  int prev = ctx->code_ctx.uco_prev_line;
  VMContext *vm_ctx = getVMContext();
  int cur_line = vm_ctx->source_map.line[vm_ctx->pc];

  GtkTreeIter iter;
  if (prev >= 0 &&
      gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, prev)) {
    gtk_list_store_set(store, &iter, 3, NULL, -1);
  }

  if (cur_line >= 0 && gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store),
                                                     &iter, NULL, cur_line)) {
    gtk_list_store_set(store, &iter, 3, "#ff0000", -1);
  }

  ctx->code_ctx.uco_prev_line = cur_line;
}