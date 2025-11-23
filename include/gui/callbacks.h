#pragma once

#include <gtk/gtk.h>

// 파일 선택
void onFileChosen(GtkFileChooserButton *chooser);

// ucode view
gboolean onKeyPress(GtkWidget *widget, GdkEventKey *event);
void highlightLine(GtkTextView *view, int line_number);
void highlightRow();
void loadUcoToTable(const char *filename);
int loadTableToUco(char ***lines, int *line_count);

// 실행
void onRun(GtkButton *button);
void updateVM(int step);

// 입출력
void guiIoWrite(int data);
int guiIoRead();
void guiIoWrite(int data);
void guiIoLf();
gboolean onEnterPress(GtkWidget *widget, GdkEventKey *event);
