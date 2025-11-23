#pragma once

#include "gui/gui_context.h"
#include "runner.h"
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

GtkWidget *initUcodeView();
void loadUcoToTable(const char *filename);
int loadTableToUco(char ***lines, int *line_count);

TextScrollInfo initAssembleView();
void setAssembleView();

GtkWidget *createNotebookView(NotebookPageInfo *pages, int page_count);
GtkWidget *createStepControl();
GtkWidget *createRegTable();

GtkWidget *createCPUStackTable();
GtkWidget *createMemoryStackTable();
GtkWidget *createLabelTable();
GtkWidget *createSymbolTable();
void updateLabelsView();
void updateSymbolsView();
void updateCPUStackView(Snapshot *snap);
void updateMemoryView(Snapshot *snap);

GtkWidget *initStatisticsBox();
void updateStatisticsBox();

// 여긴 콜백함수(분리할수도 있음 파일 분리는 고민 중)
void onFileChosen(GtkFileChooserButton *chooser);

void onOpenUco();
void onOpenLst();
void onSaveUco();
void onSaveAsUco();
void onSaveLst();

// 이름 헷갈림 변경해야 할듯
void updateLabel(GtkLabel *label, GtkAdjustment *adjustment);
void onScaleChanged(GtkRange *range, gpointer user_data);
void onFirstClicked(GtkButton *button, gpointer user_data);
void onPrevClicked(GtkButton *button, gpointer user_data);
void onNextClicked(GtkButton *button, gpointer user_data);
void onLastClicked(GtkButton *button, gpointer user_data);
void onPlayToggled(GtkToggleButton *button, gpointer user_data);