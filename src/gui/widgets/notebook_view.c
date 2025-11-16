#include "gui.h"

GtkWidget *createNotebookView() {
  GtkWidget *notebook = gtk_notebook_new();

  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

  GtkWidget *label1 = gtk_label_new("첫 번째 페이지 내용");
  GtkWidget *tab1 = gtk_label_new("uCode");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label1, tab1);

  GtkWidget *label2 = gtk_label_new("두 번째 페이지 내용");
  GtkWidget *tab2 = gtk_label_new("assemble 결과");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label2, tab2);
  return notebook;
}