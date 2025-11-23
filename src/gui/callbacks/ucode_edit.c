#include "gui/gui.h"

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