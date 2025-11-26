#include "gui/gui_widgets.h"

#include "core/vm_context.h"

GtkWidget *initLabelsView() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  GtkWidget *label = gtk_label_new("Labels");
  GtkListStore *labels_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);

  GtkWidget *treeview =
      gtk_tree_view_new_with_model(GTK_TREE_MODEL(labels_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);
  gtk_tree_view_set_grid_lines(treeview, GTK_TREE_VIEW_GRID_LINES_BOTH);

  GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

  GtkTreeIter iter;
  gtk_list_store_append(labels_store, &iter);
  gtk_list_store_set(labels_store, &iter, 0, "", 1, 0, -1);

  const char *col_names[] = {"Name", "Addr"};

  for (int i = 0; i < 2; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "xalign", 0.5, NULL);
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        col_names[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
  }

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);

  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);

  getGuiContext()->status_ctx.labels_store = labels_store;
  return box;
}

void updateLabelsView() {
  VMContext *ctx = getVMContext();

  GtkListStore *store = getGuiContext()->status_ctx.labels_store;
  gtk_list_store_clear(store);

  GtkTreeIter iter;
  for (int i = 0; i < ctx->label_list.count; i++) {
    Label *label = &ctx->label_list.labels[i];
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, label->name, 1, label->addr, -1);
  }
}
