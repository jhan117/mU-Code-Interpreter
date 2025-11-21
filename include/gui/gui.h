#pragma once

#include "gui/gui_context.h"
#include <gtk/gtk.h>

typedef struct {
  const char *tab_title; // 탭 이름
  GtkWidget *child;      // 페이지에 들어갈 위젯
} NotebookPageInfo;

typedef struct {
  GtkWidget *text_view;
  GtkWidget *scroll;
} TextScrollInfo;

void onActivate(GtkApplication *app, gpointer user_data);
GtkWidget *createLeftPanel();
GtkWidget *createRightPanel();

GtkWidget *createMenubar();
GtkWidget *createFileChooser();
char *getTextFromView(GtkTextView *view); // 유틸
TextScrollInfo createTextView();
GtkWidget *createNotebookView(NotebookPageInfo *pages, int page_count);
GtkWidget *createStepControl();
GtkWidget *createRegTable();
GtkWidget *createViewTable(const char *label_text);
void addRow(const char *text);

// 여긴 콜백함수(분리할수도 있음 파일 분리는 고민 중)
void onFileChosen(GtkFileChooserButton *chooser);

void onOpenUco();
void onOpenLst();
void onSaveUco();
void onSaveAsUco();
void onSaveLst();

void updateLabel(GtkLabel *label, GtkAdjustment *adjustment);
void onScaleChanged(GtkRange *range, gpointer user_data);
void onFirstClicked(GtkButton *button, gpointer user_data);
void onPrevClicked(GtkButton *button, gpointer user_data);
void onNextClicked(GtkButton *button, gpointer user_data);
void onLastClicked(GtkButton *button, gpointer user_data);
void onPlayToggled(GtkToggleButton *button, gpointer user_data);