#include "gui/gui.h"

#include "assemble.h"
#include "core/vm_context.h"
#include "gui/callbacks.h"
#include "io_utils/io_utils.h"

static void addColumn(GtkWidget *tree_view, GtkListStore *store,
                      const char *title, int col_idx,
                      void (*onEdited)(GtkCellRendererText *, gchar *path,
                                       gchar *new_text, gpointer user_data));
static void onLabelEdited(GtkCellRendererText *renderer, gchar *path_char,
                          gchar *new_text, gpointer user_data);
static void onOperatorEdited(GtkCellRendererText *renderer, gchar *path,
                             gchar *new_text, gpointer user_data);
static void onOperandEdited(GtkCellRendererText *renderer, gchar *path,
                            gchar *new_text, gpointer user_data);

void loadUcoToTable(const char *filename);
int loadTableToUco(char ***lines, int *line_count);

// ucode 화면 초기화
GtkWidget *initUcodeView() {
  GuiContext *ctx = getGuiContext();
  if (!ctx->ucode_table)
    ctx->ucode_table = g_malloc0(sizeof(TableInfo));

  GtkListStore *store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,
                                           G_TYPE_STRING, G_TYPE_STRING);

  // 첫 빈 행 추가
  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "", 1, "", 2, "", -1);

  // tree view 생성
  GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(tree_view),
                               GTK_TREE_VIEW_GRID_LINES_BOTH);

  addColumn(tree_view, store, "Label", 0, onLabelEdited);
  addColumn(tree_view, store, "Operator", 1, onOperatorEdited);
  addColumn(tree_view, store, "Operand", 2, onOperandEdited);

  // 스크롤 추가
  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled), tree_view);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  // 행 추가/삭제 키 입력 이벤트
  g_signal_connect(tree_view, "key-press-event", G_CALLBACK(onKeyPress), NULL);

  ctx->ucode_table->list_data = store;
  ctx->ucode_table->tree_view = tree_view;
  return scrolled;
}

static void addColumn(GtkWidget *tree_view, GtkListStore *store,
                      const char *title, int col_idx,
                      void (*onEdited)(GtkCellRendererText *, gchar *path,
                                       gchar *new_text, gpointer user_data)) {
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "editable", TRUE, NULL);

  // 열 추가
  GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
      title, renderer, "text", col_idx, "background", 3, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col);

  // 글자 수 제한 이벤트
  g_signal_connect(renderer, "edited", G_CALLBACK(onEdited), store);
}
static void onLabelEdited(GtkCellRendererText *renderer, gchar *path_text,
                          gchar *new_text, gpointer user_data) {
  const int max_chars = MAX_LABEL_LEN - 1;
  GtkListStore *store = GTK_LIST_STORE(user_data);

  gchar *final = new_text;
  if (max_chars > 0 && g_utf8_strlen(new_text, -1) > max_chars) {
    final = g_utf8_substring(new_text, 0, max_chars);
  }

  GtkTreeIter iter;
  GtkTreePath *path = gtk_tree_path_new_from_string(path_text);
  gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);

  gtk_list_store_set(store, &iter, 0, final, -1);

  if (final != new_text)
    g_free(final);
  gtk_tree_path_free(path);
}
static void onOperatorEdited(GtkCellRendererText *renderer, gchar *path_text,
                             gchar *new_text, gpointer user_data) {
  const int max_chars = MAX_CMD_LEN - 1;
  GtkListStore *store = GTK_LIST_STORE(user_data);

  gchar *final = new_text;
  if (max_chars > 0 && g_utf8_strlen(new_text, -1) > max_chars) {
    final = g_utf8_substring(new_text, 0, max_chars);
  }

  GtkTreeIter iter;
  GtkTreePath *path = gtk_tree_path_new_from_string(path_text);
  gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);

  gtk_list_store_set(store, &iter, 1, final, -1);

  if (final != new_text)
    g_free(final);
  gtk_tree_path_free(path);
}
static void onOperandEdited(GtkCellRendererText *renderer, gchar *path_text,
                            gchar *new_text, gpointer user_data) {
  GtkListStore *store = GTK_LIST_STORE(user_data);

  GtkTreeIter iter;
  GtkTreePath *path = gtk_tree_path_new_from_string(path_text);
  gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);

  gtk_list_store_set(store, &iter, 2, new_text, -1);

  gtk_tree_path_free(path);
}

void loadUcoToTable(const char *filename) {
  GuiContext *ctx = getGuiContext();
  GtkListStore *store = ctx->ucode_table->list_data;

  gtk_list_store_clear(store);

  char **lines = NULL;
  int line_count = 0;
  if (!loadUco(filename, &lines, &line_count))
    return;

  for (int i = 0; i < line_count; i++) {
    char label[MAX_LABEL_LEN] = "";
    char opcode[MAX_CMD_LEN] = "";
    char *operands[4] = {0};
    int operand_count = 0;

    int err = parseLine(lines[i], label, opcode, operands, &operand_count);
    if (err != ASSEMBLE_OK)
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
  GtkListStore *store = getGuiContext()->ucode_table->list_data;
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