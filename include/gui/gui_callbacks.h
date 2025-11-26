#pragma once

#include "gui/gui_context.h"
#include <gtk/gtk.h>

// file_io.c
typedef struct {
  int value;     // 사용자가 입력한 값
  gboolean done; // 입력이 완료되었는지
  GMutex mutex; // 워커 스레드와 메인 스레드 간 동기화용 뮤텍스
  GCond cond; // 워커 스레드가 입력 완료를 기다릴 때 사용하는 조건 변수
} IoRequest; // 스레드 전용 입출력
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

// worker_thread.c
typedef struct {
  char **lines;
  int line_count;
} WorkerData; // 스레드 작업 전용
void startWorker(WorkerData *wd);

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
void guiIoWrite(const char *data);
int guiIoRead();
void guiIoLf();
