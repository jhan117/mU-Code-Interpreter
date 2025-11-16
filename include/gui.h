#pragma once

#include <gtk/gtk.h>

void onActivate(GtkApplication *app, gpointer user_data);
GtkWidget *createLeftPanel();
GtkWidget *createRightPanel();

GtkWidget *createMenubar();
GtkWidget *createFileChooser();
GtkWidget *createNotebookView();
GtkWidget *createStepControl();