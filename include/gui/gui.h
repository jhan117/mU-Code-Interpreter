#pragma once

#include "gui/gui_context.h"
#include "runner.h"
#include <gtk/gtk.h>

void onActivate(GtkApplication *app, gpointer user_data);
GtkWidget *createLeftPanel();
GtkWidget *createRightPanel();

GtkWidget *createMenubar();

TextScrollInfo initAssembleView();
void setAssembleView();

GtkWidget *createNotebookView(NotebookPageInfo *pages, int page_count);

GtkWidget *createStepControl();
void updateStep();

GtkWidget *createRegTable();
void updateRegTable(Snapshot *snap);

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