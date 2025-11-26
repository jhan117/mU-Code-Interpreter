#pragma once

#include "gui_types.h"
#include <gtk/gtk.h>

typedef struct {
  GtkWidget *main_window;

  int is_run_done;
  RunDialog run_dialog;

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