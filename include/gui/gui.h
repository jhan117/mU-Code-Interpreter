#pragma once

#include "gui/gui_callbacks.h"
#include "gui/gui_context.h"
#include "gui/gui_widgets.h"
#include <gtk/gtk.h>

// main_window.c
int runAsGUI(int argc, char **argv);

// layout.c
void onActivate(GtkApplication *app, gpointer user_data);

// left_panel.c
GtkWidget *createLeftPanel();

// right_panel.c
GtkWidget *createRightPanel();

// menubar.c
GtkWidget *createMenubar();
