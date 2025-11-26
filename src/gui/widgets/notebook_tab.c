#include "gui/gui_widgets.h"

GtkWidget *createNotebookTab(NotebookPageInfo *pages, int page_count) {
  GtkWidget *notebook = gtk_notebook_new();
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

  for (int i = 0; i < page_count; i++) {
    GtkWidget *tab = gtk_label_new(pages[i].tab_title);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pages[i].child, tab);
  }

  return notebook;
}