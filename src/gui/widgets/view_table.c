#include "gui/gui.h"

// 임시로 전역 데이터
GtkListStore *store;

void addRow(const char *text) {
  // 추후에 수정
  GtkTreeIter iter;
  gtk_list_store_insert(store, &iter, 0); // 맨 끝에 삽입 (다 스택이라)
  gtk_list_store_set(store, &iter, 0, text, -1);
}

GtkWidget *createRegTable() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  GtkWidget *label = gtk_label_new("CPU Registers");
  GtkListStore *regStore = gtk_list_store_new(6,
                                              G_TYPE_INT, // cs
                                              G_TYPE_INT, // pc
                                              G_TYPE_INT, // ds
                                              G_TYPE_INT, // ss
                                              G_TYPE_INT, // sp
                                              G_TYPE_INT  // bp
  );
  GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(regStore));
  GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

  GtkTreeIter iter;
  gtk_list_store_append(regStore, &iter);
  // 임시 값으로 보여주기
  gtk_list_store_set(regStore, &iter, // 값 시작
                     0, 0,            // cs
                     1, 0,            // pc
                     2, 0,            // ds
                     3, 0,            // ss
                     4, 0,            // sp
                     5, 0,            // bp
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

  return box;
}

GtkWidget *createViewTable(const char *label_text, int isStack) {
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

  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);

  return box;
}