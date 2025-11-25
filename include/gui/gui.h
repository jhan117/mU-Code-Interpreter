#pragma once

#include "gui/gui_callbacks.h"
#include "gui/gui_context.h"
#include "gui/gui_widgets.h"
#include <gtk/gtk.h>

void onActivate(GtkApplication *app, gpointer user_data);
GtkWidget *createLeftPanel();
GtkWidget *createRightPanel();