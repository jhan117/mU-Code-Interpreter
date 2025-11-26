#pragma once

#include "gui/gui_context.h"
#include <gtk/gtk.h>

// file_io.c
// menu의 open, save 추가
void onFileChosen(GtkFileChooserButton *chooser);
void onOpenUco();
void onSaveUco();
void onSaveAsUco();
void onOpenLst();
void onSaveLst();

// ucode editor.c
GtkCellRenderer *addColumn(GtkWidget *tree_view, GtkListStore *store,
                           const char *title, int col_idx,
                           void (*onEdited)(GtkCellRendererText *, gchar *path,
                                            gchar *new_text,
                                            gpointer user_data));
void onLabelEdited(GtkCellRendererText *renderer, gchar *path_text,
                   gchar *new_text, gpointer user_data);
void onOperatorEdited(GtkCellRendererText *renderer, gchar *path_text,
                      gchar *new_text, gpointer user_data);
void onOperandEdited(GtkCellRendererText *renderer, gchar *path_text,
                     gchar *new_text, gpointer user_data);
gboolean onKeyPress(GtkWidget *widget, GdkEventKey *event); // 기능 수정 필요

// step_control.c
void onPlayToggled(GtkToggleButton *button);
void onScaleChanged(GtkRange *range);
void onFirstClicked(GtkButton *button);
void onPrevClicked(GtkButton *button);
void onNextClicked(GtkButton *button);
void onLastClicked(GtkButton *button);
void onIconToggled(GtkToggleButton *button, gpointer user_data);

// vm_runner.c
void onRun(GtkButton *button);

// status_visible.c
void onToggleRegisters(GtkWidget *widget, gpointer user_data);
void onToggleCPU(GtkWidget *widget, gpointer user_data);
void onToggleMemory(GtkWidget *widget, gpointer user_data);
void onToggleLabels(GtkWidget *widget, gpointer user_data);
void onToggleSymbols(GtkWidget *widget, gpointer user_data);
void onToggleStatistics(GtkWidget *widget, gpointer user_data);

// io.c
void onInsertText(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text,
                  gint len, gpointer user_data);
void guiIoWrite(int data);
int guiIoRead();
void guiIoLf();
