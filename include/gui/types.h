#pragma once

#include <gtk/gtk.h>

typedef struct {
  const char *tab_title; // 탭 이름
  GtkWidget *child;      // 페이지에 들어갈 위젯
} NotebookPageInfo;

typedef struct {
  GtkWidget *tree_view;    // 트리 뷰 (테이블)
  GtkListStore *list_data; // 트리 뷰에 저장된 리스트 데이터
} TableInfo;

typedef struct {
  GtkWidget *text_view; // 텍스트 뷰
  GtkWidget *scroll;    // 감싸는 스크롤 위젯
} TextScrollInfo;