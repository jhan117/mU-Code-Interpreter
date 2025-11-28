#pragma once

#include "core/types.h" // Snapshot
#include "gui/gui_callbacks.h"
#include "gui/gui_context.h"
#include <gtk/gtk.h>

// file_chooser.c
GtkWidget *createFileChooser();

// ucode_view.c (callback x)
TextScrollInfo createUcodeView();
void updateUcodeView(char *content);
int getUcodeView(char ***lines, int *line_count);
void freeUcoView(char **lines, int line_count);

// assemble_view.c (callback x)
TextScrollInfo createAssembleView();
void updateAssembleView();

// step_control.c
GtkWidget *createStepControl();
void initStep();
void updateStep();

// notebook_tab.c
GtkWidget *createNotebookTab(NotebookPageInfo *pages, int page_count);

// status_view.c (callback x)
GtkWidget *createStatusView();
void updateStatusView();

// reg_view.c (callback x)
GtkWidget *createRegView();
void updateRegView();

// cpu_view.c (callback x)
GtkWidget *createCPUView();
void updateCPUView();

// memory_view.c (callback x)
GtkWidget *createMemoryView();
void updateMemoryView();

// labels_view.c (callback x)
GtkWidget *createLabelsView();
void updateLabelsView();

// symbols_view.c (callback x)
GtkWidget *createSymbolsView();
void updateSymbolsView();

// statistics_view.c (callback x)
GtkWidget *createStatisticsView();
void updateStatisticsView();

// io_view.c
TextScrollInfo createIOView();

// lst_view.c
TextScrollInfo createLstView();
void updateLstView();

// utils.c
void toggleWidgetsVisible(int is_true);
void resetAllText(GtkWidget *text_view);
void insertAtEnd(GtkWidget *text_view, const char *text);
void highlightLine(GtkWidget *text_View, int line);

// message_diag.c
void showMessage(GtkMessageType type, const char *message);
