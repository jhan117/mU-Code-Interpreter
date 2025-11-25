#pragma once

#include "gui/gui_context.h"
#include <gtk/gtk.h>

// file_io.c
void onFileChosen(GtkFileChooserButton *chooser);

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
void onFirstClicked(GtkButton *button);
void onPrevClicked(GtkButton *button);
void onNextClicked(GtkButton *button);
void onLastClicked(GtkButton *button);
void onIconToggled(GtkToggleButton *button);

// 실행
void onRun(GtkButton *button);
void updateVM(int step);

// io.c
void onInsertText(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text,
                  gint len, gpointer user_data);
void guiIoWrite(int data);
int guiIoRead();
void guiIoLf();
