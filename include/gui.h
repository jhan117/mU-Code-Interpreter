#pragma once

#include <gtk/gtk.h>

typedef struct {
  const char *tab_title; // 탭 이름
  GtkWidget *child;      // 페이지에 들어갈 위젯
} NotebookPageInfo;

void onActivate(GtkApplication *app, gpointer user_data);
GtkWidget *createLeftPanel();
GtkWidget *createRightPanel();

GtkWidget *createMenubar();
GtkWidget *createFileChooser();
GtkWidget *createNotebookView(NotebookPageInfo *pages, int page_count);
GtkWidget *createStepControl();
GtkWidget *createViewTable(const char *label_text);
void addRow(const char *text);