#include "gui/gui_widgets.h"

#include "core/vm_context.h"

GtkWidget *createMemoryView() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  GtkWidget *label = gtk_label_new("Memory");
  GtkListStore *memory_store = gtk_list_store_new(1, G_TYPE_INT);

  GtkWidget *treeview =
      gtk_tree_view_new_with_model(GTK_TREE_MODEL(memory_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);
  gtk_tree_view_set_grid_lines(treeview, GTK_TREE_VIEW_GRID_LINES_BOTH);

  GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

  GtkTreeIter iter;
  gtk_list_store_append(memory_store, &iter);
  gtk_list_store_set(memory_store, &iter, 0, 0, -1);

  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 0.5, NULL);
  GtkTreeViewColumn *column =
      gtk_tree_view_column_new_with_attributes("", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);

  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);

  getGuiContext()->status_ctx.memory_store = memory_store;
  return box;
}

void updateMemoryView(const Snapshot *snap) {
  GtkListStore *store = getGuiContext()->status_ctx.memory_store;
  gtk_list_store_clear(store);

  GtkTreeIter iter;
  VMContext *ctx = getVMContext();
  for (int i = ctx->sp + 1; i < INIT_MEMORY_SIZE; i++) {
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, ctx->memory[i], -1);
  }
}
