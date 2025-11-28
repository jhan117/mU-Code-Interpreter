#pragma once

#include "gui_types.h"
#include <gtk/gtk.h>

typedef struct {
  GtkWidget *main_window;

  // 왼쪽 패널
  FileContext file_ctx;
  CodeContext code_ctx;
  StepContext step_ctx;
  GtkWidget *run_btn;

  // 오른쪽 상중단 패널
  StatusContext status_ctx;
  StatContext stat_ctx;

  // 오른쪽 하단 패널
  GtkWidget *lst_view;
  IOContext io_ctx;
} GuiContext;

GuiContext *getGuiContext();