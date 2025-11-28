#include "gui/gui.h"

static GtkWidget *createMenuItem(const Menu *menu, gboolean checkable) {
  GtkWidget *menu_item;

  // leaf가 아닌 최상위/중간 메뉴는 그냥 일반 메뉴 아이템
  if (!checkable) {
    menu_item = gtk_menu_item_new_with_label(menu->label);
  } else {
    // View 메뉴의 자식만 체크박스
    menu_item = gtk_check_menu_item_new_with_label(menu->label);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_item), TRUE);
  }

  // 자식이 있으면 재귀적으로 생성
  if (menu->child_count > 0) {
    GtkWidget *submenu = gtk_menu_new();
    for (int i = 0; i < menu->child_count; i++) {
      gboolean child_checkable = (strcmp(menu->label, "View") == 0);
      GtkWidget *child_item = createMenuItem(&menu->child[i], child_checkable);
      gtk_menu_shell_append(GTK_MENU_SHELL(submenu), child_item);
    }
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), submenu);
  } else {
    // leaf 노드일 경우 콜백 연결
    if (menu->callback != NULL) {
      if (checkable) {
        g_signal_connect(menu_item, "toggled", G_CALLBACK(menu->callback),
                         NULL);
      } else {
        g_signal_connect(menu_item, "activate", G_CALLBACK(menu->callback),
                         NULL);
      }
    }
  }

  return menu_item;
}

GtkWidget *createMenubar() {
  const Menu file_open_child[] = {
      {"Open .uco", NULL, 0, onOpenUco},
  };
  const Menu file_save_child[] = {
      {"Save .uco", NULL, 0, onSaveUco},
      {"Save as .uco", NULL, 0, onSaveAsUco},
      {"Save .lst", NULL, 0, onSaveLst},
  };
  const Menu file_child[] = {
      {"Open", file_open_child, 1, NULL},
      {"Save", file_save_child, 3, NULL},
  };
  const Menu run_child[] = {
      {"Run", NULL, 0, onRun},
  };
  const Menu view_child[] = {
      {"Registers", NULL, 0, onToggleRegisters},
      {"Memory", NULL, 0, onToggleMemory},
      {"CPU Stack", NULL, 0, onToggleCPU},
      {"Labels", NULL, 0, onToggleLabels},
      {"Symbols", NULL, 0, onToggleSymbols},
  };
  const Menu help_child[] = {
      {"GUI 사용법", NULL, 0, onHelpUsage},
      {"명령어 설명", NULL, 0, onHelpInstruction},
      {"About Team", NULL, 0, onHelpAbout},
  };
  const Menu menus[] = {
      {"File", file_child, 2, NULL},
      {"Run", run_child, 1, NULL},
      {"View", view_child, 5, NULL},
      {"Help", help_child, 3, NULL},
  };
  const int menu_count = sizeof(menus) / sizeof(menus[0]);

  GtkWidget *menubar = gtk_menu_bar_new();

  for (int i = 0; i < menu_count; i++) {
    GtkWidget *item = createMenuItem(&menus[i], FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item);
  }

  return menubar;
}
