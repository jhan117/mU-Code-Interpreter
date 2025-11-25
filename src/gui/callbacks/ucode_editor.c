#include "gui/gui.h"

#include "assembler/assemble_error.h"
#include "io_utils/io_utils.h"

// 키로 행 추가/삭제
gboolean onKeyPress(GtkWidget *widget, GdkEventKey *event) {
  GtkTreeView *tree = GTK_TREE_VIEW(widget);
  GtkTreeSelection *selection = gtk_tree_view_get_selection(tree);
  GtkTreeIter iter;
  GtkTreeModel *model;

  if (!gtk_tree_selection_get_selected(selection, &model, &iter))
    return FALSE;

  switch (event->keyval) {
    // 엔터로 행 추가
  case GDK_KEY_Return:
  case GDK_KEY_KP_Enter: {
    GtkTreeIter new_iter;
    if (!gtk_tree_model_iter_next(model, &iter)) {
      gtk_list_store_append(GTK_LIST_STORE(model), &new_iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &new_iter, 0, "", 1, "", 2, "",
                         -1);
    } else {
      new_iter = iter;
    }
    gtk_tree_selection_select_iter(selection, &new_iter);

    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(tree));
    gtk_tree_view_set_cursor_on_cell(
        GTK_TREE_VIEW(tree), gtk_tree_model_get_path(model, &new_iter),
        GTK_TREE_VIEW_COLUMN(g_list_nth_data(columns, 0)), NULL, TRUE);

    g_list_free(columns);
    return TRUE;
  }

    // 백스페이스로 삭제 (최소 1행은 유지)
  case GDK_KEY_BackSpace: {
    GtkTreeModel *model = GTK_TREE_MODEL(gtk_tree_view_get_model(tree));
    gint n_rows = gtk_tree_model_iter_n_children(model, NULL);

    if (n_rows <= 1)
      break;

    gchar *label = NULL;
    gchar *op = NULL;
    gchar *operand = NULL;

    gtk_tree_model_get(model, &iter, 0, &label, 1, &op, 2, &operand, -1);

    if ((!label || *label == '\0') && (!op || *op == '\0') &&
        (!operand || *operand == '\0')) {
      gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
    }

    g_free(label);
    g_free(op);
    g_free(operand);

    return TRUE;
  }

  default:
    break;
  }

  return FALSE;
}

void loadUcoToTable(const char *filename) {
  GuiContext *ctx = getGuiContext();
  GtkListStore *store = ctx->code_ctx.ucode_table.list_data;

  gtk_list_store_clear(store);

  char **lines = NULL;
  int line_count = 0;
  if (!loadUco(filename, &lines, &line_count))
    return;

  for (int i = 0; i < line_count; i++) {
    char label[MAX_LABEL_LEN] = "";
    char opcode[MAX_OP_LEN] = "";
    char *operands[4] = {0};
    int operand_count = 0;

    int err = parseLine(lines[i], label, opcode, operands, &operand_count);
    if (err != ASSEMBLE_ERR_NONE)
      continue;

    char operand_str[256] = "";
    for (int j = 0; j < operand_count; j++) {
      strcat(operand_str, operands[j]);
      if (j != operand_count - 1)
        strcat(operand_str, " ");
      free(operands[j]);
    }

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, label, 1, opcode, 2, operand_str, -1);
  }

  freeUco(lines, line_count);
  ctx->current_step = 0;
}
int loadTableToUco(char ***lines, int *line_count) {
  GtkListStore *store = getGuiContext()->code_ctx.ucode_table.list_data;
  GtkTreeModel *model = GTK_TREE_MODEL(store);
  GtkTreeIter iter;
  gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

  int capacity = INIT_LINE_CAPACITY;
  *lines = malloc(sizeof(char *) * capacity);
  if (!*lines)
    return 0;
  *line_count = 0;
  char buffer[256];

  while (valid) {
    char *label = NULL;
    char *operator= NULL;
    char *operand = NULL;

    gtk_tree_model_get(model, &iter, 0, &label, 1, &operator, 2, &operand, -1);

    if (!label && !operator&& !operand)
      break;

    snprintf(buffer, sizeof(buffer), "%-10.10s %s %s\n",
                          label ? label : "",
                      operator ? operator : "",
                      operand ? operand : "");

    if (*line_count >= capacity) {
      capacity *= 2;
      char **tmp = realloc((*lines), sizeof(char *) * capacity);
      if (!tmp) {
        for (int i = 0; i < (*line_count); i++)
          free((*lines)[i]);
        free((*lines));
        g_free(label);
        g_free(operator);
        g_free(operand);
        return 0;
      }
      (*lines) = tmp;
    }

    (*lines)[*line_count] = malloc(strlen(buffer) + 1);
    if ((*lines)[*line_count]) {
      strcpy((*lines)[*line_count], buffer);
      (*line_count)++;
    }

    g_free(label);
    g_free(operator);
    g_free(operand);

    valid = gtk_tree_model_iter_next(model, &iter);
  }

  return 1;
}