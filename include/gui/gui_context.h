#pragma once

#include <gtk/gtk.h>

// 이 친구 모든 위젯을 들고 있어야 하나... 많아져서 보기 싫은데
typedef struct {
  GtkWidget *main_window;

  int current_step;
  char *uco_filename;
  char *lst_filename;

  GtkWidget *file_chooser;
  GtkWidget *ucode_view;
  GtkWidget *assemble_view;
  GtkWidget *lst_view;

  GtkWidget *btn_first;
  GtkWidget *btn_prev;
  GtkWidget *btn_toggle;
  GtkWidget *btn_next;
  GtkWidget *btn_last;

  GtkListStore *cpu_stack_store;
  GtkListStore *memory_store;

  GtkWidget *io_view;
} GuiContext;

GuiContext *getGuiContext(void);