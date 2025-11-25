#include "gui/gui.h"

#include "core/vm_context.h"
#include "gui/gui_callbacks.h"
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

GtkWidget *initUcodeView() {
  GtkListStore *store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,
                                           G_TYPE_STRING, G_TYPE_STRING);

  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "", 1, "", 2, "", -1);

  GtkWidget *tree_view = gtk_tree_view_new_with_model(store);
  gtk_tree_view_set_grid_lines(tree_view, GTK_TREE_VIEW_GRID_LINES_BOTH);

  addColumn(tree_view, store, "Label", 0, onLabelEdited);
  addColumn(tree_view, store, "Operator", 1, onOperatorEdited);
  addColumn(tree_view, store, "Operand", 2, onOperandEdited);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled), tree_view);
  gtk_scrolled_window_set_policy(scrolled, GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  // 행 추가/삭제 키 입력 이벤트
  g_signal_connect(tree_view, "key-press-event", G_CALLBACK(onKeyPress), NULL);

  GuiContext *ctx = getGuiContext();
  ctx->code_ctx.ucode_table.tree_view = tree_view;
  ctx->code_ctx.ucode_table.list_data = store;

  return scrolled;
}

static void addColumn(GtkWidget *tree_view, GtkListStore *store,
                      const char *title, int col_idx,
                      void (*onEdited)(GtkCellRendererText *, gchar *path,
                                       gchar *new_text, gpointer user_data)) {
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "editable", TRUE, NULL);

  GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
      title, renderer, "text", col_idx, "background", 3, NULL);
  gtk_tree_view_append_column(tree_view, col);

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
  gtk_tree_model_get_iter(store, &iter, path);
  gtk_list_store_set(store, &iter, 0, final, -1);

  if (final != new_text)
    g_free(final);

  gtk_tree_path_free(path);
}
static void onOperatorEdited(GtkCellRendererText *renderer, gchar *path_text,
                             gchar *new_text, gpointer user_data) {
  const int max_chars = MAX_OP_LEN - 1;
  GtkListStore *store = GTK_LIST_STORE(user_data);

  gchar *final = new_text;
  if (max_chars > 0 && g_utf8_strlen(new_text, -1) > max_chars) {
    final = g_utf8_substring(new_text, 0, max_chars);
  }

  GtkTreeIter iter;
  GtkTreePath *path = gtk_tree_path_new_from_string(path_text);
  gtk_tree_model_get_iter(store, &iter, path);

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
  gtk_tree_model_get_iter(store, &iter, path);

  gtk_list_store_set(store, &iter, 2, new_text, -1);

  gtk_tree_path_free(path);
}
