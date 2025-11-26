#include "gui/gui_widgets.h"

GtkWidget *initRegView() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  gtk_widget_set_halign(box, GTK_ALIGN_START);

  GtkWidget *label = gtk_label_new("CPU Registers");
  GtkListStore *reg_store = gtk_list_store_new(6,
                                               G_TYPE_INT, // cs
                                               G_TYPE_INT, // pc
                                               G_TYPE_INT, // ds
                                               G_TYPE_INT, // ss
                                               G_TYPE_INT, // sp
                                               G_TYPE_INT  // bp
  );
  GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(reg_store));
  gtk_tree_view_set_grid_lines(treeview, GTK_TREE_VIEW_GRID_LINES_BOTH);

  GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);

  GtkTreeIter iter;
  gtk_list_store_append(reg_store, &iter);
  gtk_list_store_set(reg_store, &iter, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, -1);

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

  getGuiContext()->status_ctx.reg_store = reg_store;
  return box;
}

void updateRegView(Snapshot *snap) {
  if (!snap)
    return;

  GtkListStore *store = getGuiContext()->status_ctx.reg_store;
  gtk_list_store_clear(store);

  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, snap->cs, 1, snap->pc, 2, snap->ds, 3,
                     snap->ss, 4, snap->sp, 5, snap->bp, -1);
}