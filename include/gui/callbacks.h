#pragma once

#include <gtk/gtk.h>

// 파일 선택
void onFileChosen(GtkFileChooserButton *chooser);

// ucode view
gboolean onKeyPress(GtkWidget *widget, GdkEventKey *event);

// 실행
void onRun(GtkButton *button);
