#pragma once

#include <gtk/gtk.h>

typedef struct {
  GtkWidget *tree_view;
  GtkListStore *list_data;
} TableInfo;

// 이 친구 모든 위젯을 들고 있어야 하나... 많아져서 보기 싫은데
typedef struct {
  GtkWidget *main_window;

  int is_run_done;
  int timeout_id;
  int current_step;
  char *uco_filename;
  char *lst_filename;

  int uco_prev_line;

  GtkWidget *file_chooser;
  GtkWidget *ucode_view;
  GtkWidget *assemble_view;
  GtkWidget *lst_view;

  GtkAdjustment *adj;
  GtkWidget *step_label;
  GtkWidget *btn_first;
  GtkWidget *btn_prev;
  GtkWidget *btn_toggle;
  GtkWidget *btn_next;
  GtkWidget *btn_last;

  TableInfo *ucode_table;
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