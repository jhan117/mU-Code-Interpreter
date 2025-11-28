#pragma once

#include <gtk/gtk.h>

typedef void (*MenuCallback)(GtkWidget *, gpointer);

typedef struct Menu {
  const char *label;     // 메뉴 이름
  struct Menu *child;    // 하위 메뉴 배열
  int child_count;       // 하위 메뉴 개수
  MenuCallback callback; // 콜백 함수 포인터
} Menu;

typedef struct {
  const char *tab_title; // 탭 이름
  GtkWidget *child;      // 페이지에 들어갈 위젯
} NotebookPageInfo;

typedef struct {
  GtkWidget *tree_view;    // 트리 뷰 (테이블)
  GtkListStore *list_data; // 트리 뷰에 저장된 리스트 데이터
  GtkCellRenderer *renderer[3];
} TableInfo;

typedef struct {
  GtkWidget *text_view; // 텍스트 뷰
  GtkWidget *scroll;    // 감싸는 스크롤 위젯
} TextScrollInfo;

typedef struct {
  GtkWidget *file_chooser;
  char *uco_filename;
  char *lst_filename;
} FileContext;

typedef struct {
  int uco_prev_line;
  TableInfo ucode_table;
  TextScrollInfo assemble_view;
} CodeContext;

typedef struct {
  int timeout_id;
  int current_step;

  GtkAdjustment *adj;
  GtkWidget *step_label;
  GtkWidget *btn_first;
  GtkWidget *btn_prev;
  GtkWidget *btn_toggle;
  GtkWidget *btn_next;
  GtkWidget *btn_last;
} StepContext;

typedef struct {
  GtkListStore *reg_store;
  GtkListStore *cpu_store;
  GtkListStore *memory_store;
  GtkListStore *labels_store;
  GtkListStore *symbols_store;

  GtkWidget *reg_view;
  GtkWidget *status_box;
  GtkWidget *cpu_view;
  GtkWidget *memory_view;
  GtkWidget *labels_view;
  GtkWidget *symbols_view;
} StatusContext;

typedef struct {
  GtkWidget *stat_mem_label;
  GtkListStore *stat_use_store;
  GtkListStore *stat_run_store;
} StatContext;

typedef struct {
  GtkWidget *io_view;
  GAsyncQueue *input_queue;
  int is_last_write;

  // 실행 완료 된 원본 저장
  char **lines;
  int line_count;
} IOContext;