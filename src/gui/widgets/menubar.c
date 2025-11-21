#include "gui/gui.h"

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
  } else {
    if (strcmp(menu->label, "Open .uco") == 0) {
      g_signal_connect(menu_item, "activate", G_CALLBACK(onOpenUco), NULL);
    } else if (strcmp(menu->label, "Open .lst") == 0) {
      g_signal_connect(menu_item, "activate", G_CALLBACK(onOpenLst), NULL);
    } else if (strcmp(menu->label, "Save .uco") == 0) {
      g_signal_connect(menu_item, "activate", G_CALLBACK(onSaveUco), NULL);
    } else if (strcmp(menu->label, "Save As .uco") == 0) {
      g_signal_connect(menu_item, "activate", G_CALLBACK(onSaveAsUco), NULL);
    } else if (strcmp(menu->label, "Save .lst") == 0) {
      g_signal_connect(menu_item, "activate", G_CALLBACK(onSaveLst), NULL);
    }
    // TODO: 다른 메뉴들도 같은 방식으로 연결
  }
  return menu_item;
}

GtkWidget *createMenubar() {
  Menu file_open_child[] = {
      {"Open .uco", NULL, 0},
      {"Open .lst", NULL, 0},
  };
  Menu file_save_child[] = {
      {"Save .uco", NULL, 0},
      {"Save As .uco", NULL, 0},
      {"Save .lst", NULL, 0},
  };
  Menu file_child[] = {
      {"Open", file_open_child, 2},
      {"Save", file_save_child, 3},
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
      {"File", file_child, 2},
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
