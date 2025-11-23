#pragma once

#include "gui/types.h"
#include <gtk/gtk.h>

typedef struct {
  GtkWidget *file_chooser;
  char *uco_filename;
  char *lst_filename;
} FileContext;

typedef struct {
  TableInfo ucode_table;
  TextScrollInfo assemble_view;
} CodeContext;

typedef struct {
  GtkWidget *main_window;

  FileContext file_ctx;
  CodeContext code_ctx;

  int is_run_done;
  int timeout_id;
  int current_step;
  int uco_prev_line;

  GtkWidget *lst_view;

  GtkAdjustment *adj;
  GtkWidget *step_label;
  GtkWidget *btn_first;
  GtkWidget *btn_prev;
  GtkWidget *btn_toggle;
  GtkWidget *btn_next;
  GtkWidget *btn_last;

  GtkListStore *reg_store;
  GtkListStore *cpu_stack_store;
  GtkListStore *memory_store;
  GtkListStore *labels_store;
  GtkListStore *symbols_store;

  GtkWidget *stat_mem_label;
  GtkListStore *stat_use_store;
  GtkListStore *stat_run_store;

  GtkWidget *io_view;
  int got_input;
  int value;
} GuiContext;

GuiContext *getGuiContext(void);