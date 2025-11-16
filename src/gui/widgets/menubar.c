#include "gui.h"

typedef struct Menu {
  const char *label;     // 메뉴 이름
  struct Menu *children; // 하위 메뉴 배열
  int children_count;    // 하위 메뉴 개수
} Menu;

static GtkWidget *createMenuItem(const Menu *menu) {
  GtkWidget *menu_item = gtk_menu_item_new_with_label(menu->label);

  if (menu->children_count > 0) {
    GtkWidget *submenu = gtk_menu_new();
    for (int i = 0; i < menu->children_count; i++) {
      GtkWidget *child_item = createMenuItem(&menu->children[i]);
      gtk_menu_shell_append(GTK_MENU_SHELL(submenu), child_item);
    }
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), submenu);
  }

  return menu_item;
}

GtkWidget *createMenubar() {
  Menu file_open_child[] = {
      {"Open .uco", NULL, 0},
      {"Open .lst", NULL, 0},
  };
  Menu file_save_child[] = {
      // save / save as 구분할지 말지 생각 중...
      {"Save .uco", NULL, 0},
      {"Save .lst", NULL, 0},
  };
  Menu file_child[] = {
      // new 추가할지말지 생각 중...
      {"Open", file_open_child, 2},
      {"Save", file_save_child, 2},
      // 나중에 Recent 파일 목록 동적으로 추가하기
      {"Recent", (Menu[]){{"Recent .uco files", NULL, 0}}, 1},
  };
  Menu run_child[] = {
      {"Run", NULL, 0},
  };
  Menu view_child[] = {
      {"Registers", NULL, 0}, {"Memory", NULL, 0},  {"Stack", NULL, 0},
      {"Labels", NULL, 0},    {"Symbols", NULL, 0}, {"Statistics", NULL, 0},
  };
  Menu help_child[] = {
      {"GUI 사용법", NULL, 0},
      {"About Team", NULL, 0},
  };
  Menu menus[] = {
      {"File", file_child, 3},
      {"Run", run_child, 1},
      {"View", view_child, 6},
      {"Help", help_child, 2},
  };

  GtkWidget *menubar = gtk_menu_bar_new();
  int menu_count = sizeof(menus) / sizeof(menus[0]);

  for (int i = 0; i < menu_count; i++) {
    GtkWidget *item = createMenuItem(&menus[i]);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item);
  }

  return menubar;
}