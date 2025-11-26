#pragma once

#include "core/types.h" // Snapshot
#include "gui/gui_callbacks.h"
#include "gui/gui_context.h"
#include <gtk/gtk.h>

// file_chooser.c
GtkWidget *initFileChooser();

// ucode_view.c
GtkWidget *initUcodeView();
void updateUcodeView(char ***lines, int *line_count);
int getUcodeView(char ***lines, int *line_count);
void highlightRow();

// assemble_view.c (callback x)
TextScrollInfo initAssembleView();
void updateAssembleView();
void highlightLine();

// step_control.c
GtkWidget *initStepControl();
void initStep();
void updateStep();

// notebook_tab.c
GtkWidget *initNotebookTab(NotebookPageInfo *pages, int page_count);

// runner_dialog.c
void initRunDialog(RunDialog *rd, GtkWindow *parent, const char *title,
                   const char *initial_text);
void destroyRunDialog(RunDialog *rd);

// status_view.c (callback x)
GtkWidget *initStatusView();
void updateStatusView(int step);

// reg_view.c (callback x)
GtkWidget *initRegView();
void updateRegView(Snapshot *snap);

// cpu_view.c (callback x)
GtkWidget *initCPUView();
void updateCPUView(Snapshot *snap);

// memory_view.c (callback x)
GtkWidget *initMemoryView();
void updateMemoryView(Snapshot *snap);

// labels_view.c (callback x)
GtkWidget *initLabelsView();
void updateLabelsView();

// symbols_view.c (callback x)
GtkWidget *initSymbolsView();
void updateSymbolsView();

// statistics_view.c (callback x)
GtkWidget *initStatisticsView();
void updateStatisticsView();

// io_view.c
GtkWidget *initIOView();

// utils.c
void toggleWidgetsStatus(int is_true);
void resetAllText(GtkWidget *text_view);
void insertAtEnd(GtkWidget *text_view, const char *text);