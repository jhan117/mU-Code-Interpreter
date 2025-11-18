#include "gui.h"

// 임시로 전역 데이터
GtkListStore *store;

void addRow(const char *text) {
  // 추후에 수정
  GtkTreeIter iter;
  gtk_list_store_insert(store, &iter, 0); // 맨 끝에 삽입 (다 스택이라)
  gtk_list_store_set(store, &iter, 0, text, -1);
}

GtkWidget *createViewTable(const char *label_text) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  GtkWidget *label = gtk_label_new(label_text);

  store = gtk_list_store_new(1, G_TYPE_STRING);

  GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);

  g_object_unref(store);

  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn *column =
      gtk_tree_view_column_new_with_attributes("", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);
  gtk_widget_set_size_request(scrolled, 200, 150);

  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);

  return box;
}