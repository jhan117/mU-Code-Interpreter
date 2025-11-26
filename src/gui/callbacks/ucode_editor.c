#include "gui/gui_callbacks.h"

#include "core/constants.h" // MAX_LABEL_LEN, MAX_OP_LEN

GtkCellRenderer *addColumn(GtkWidget *tree_view, GtkListStore *store,
                           const char *title, int col_idx,
                           void (*onEdited)(GtkCellRendererText *, gchar *path,
                                            gchar *new_text,
                                            gpointer user_data)) {
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "editable", TRUE, NULL);

  GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
      title, renderer, "text", col_idx, "background", 3, NULL);
  gtk_tree_view_append_column(tree_view, col);

  // 글자 수 제한 이벤트
  g_signal_connect(renderer, "edited", G_CALLBACK(onEdited), store);

  return renderer;
}
void onLabelEdited(GtkCellRendererText *renderer, gchar *path_text,
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
void onOperatorEdited(GtkCellRendererText *renderer, gchar *path_text,
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
void onOperandEdited(GtkCellRendererText *renderer, gchar *path_text,
                     gchar *new_text, gpointer user_data) {
  GtkListStore *store = GTK_LIST_STORE(user_data);

  GtkTreeIter iter;
  GtkTreePath *path = gtk_tree_path_new_from_string(path_text);
  gtk_tree_model_get_iter(store, &iter, path);

  gtk_list_store_set(store, &iter, 2, new_text, -1);

  gtk_tree_path_free(path);
}

// 키로 행 추가/삭제
gboolean onKeyPress(GtkWidget *widget, GdkEventKey *event) {
  GtkTreeView *tree = GTK_TREE_VIEW(widget);
  GtkTreeSelection *selection = gtk_tree_view_get_selection(tree);
  GtkTreeIter iter;
  GtkTreeModel *model;

  if (!gtk_tree_selection_get_selected(selection, &model, &iter))
    return FALSE;

  switch (event->keyval) {
    // 엔터로 행 추가 => insert로 바꿀까 말까...
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

    // 백스페이스로 삭제 (최소 1행은 유지) => delete 로 바꿀까 말까
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
