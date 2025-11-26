#include "gui/gui_widgets.h"

#include "core/vm_context.h"

GtkWidget *initSymbolsView() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  GtkWidget *label = gtk_label_new("Symbols");
  GtkListStore *symbols_store =
      gtk_list_store_new(4, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

  GtkWidget *treeview =
      gtk_tree_view_new_with_model(GTK_TREE_MODEL(symbols_store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);
  gtk_tree_view_set_grid_lines(treeview, GTK_TREE_VIEW_GRID_LINES_BOTH);

  GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

  GtkTreeIter iter;
  gtk_list_store_append(symbols_store, &iter);
  gtk_list_store_set(symbols_store, &iter, 0, 0, 1, 0, 2, 0, 3, 0, -1);

  const char *col_names[] = {"Idx", "Block", "Offset", "Size"};

  for (int i = 0; i < 4; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "xalign", 0.5, NULL);
    GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
        col_names[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
  }

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);

  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), scrolled, TRUE, TRUE, 0);

  getGuiContext()->status_ctx.symbols_store = symbols_store;
  return box;
}

void updateSymbolsView() {
  VMContext *ctx = getVMContext();

  GtkListStore *store = getGuiContext()->status_ctx.symbols_store;
  gtk_list_store_clear(store);

  GtkTreeIter iter;
  for (int i = 0; i < ctx->symbol_list.count; i++) {
    Symbol *sym = &ctx->symbol_list.symbols[i];
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, sym->index, 1, sym->block, 2,
                       sym->offset, 3, sym->size, -1);
  }
}