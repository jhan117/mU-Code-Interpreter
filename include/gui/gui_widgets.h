#pragma once

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

// 수정하기
// vm_runner.c
void onRun(GtkButton *button);

// io_view.c
GtkWidget *initIOView();

// utils.c
void resetAllText(GtkWidget *text_view);
void insertAtEnd(GtkWidget *text_view, const char *text);