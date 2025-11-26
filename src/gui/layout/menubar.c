#include "gui/gui.h"

static GtkWidget *createMenuItem(const Menu *menu) {
  GtkWidget *menu_item = gtk_menu_item_new_with_label(menu->label);

  if (menu->child_count > 0) {
    GtkWidget *submenu = gtk_menu_new();
    for (int i = 0; i < menu->child_count; i++) {
      GtkWidget *child_item = createMenuItem(&menu->child[i]);
      gtk_menu_shell_append(GTK_MENU_SHELL(submenu), child_item);
    }
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), submenu);
  } else {
    if (menu->callback != NULL) {
      g_signal_connect(menu_item, "activate", G_CALLBACK(menu->callback), NULL);
    }
  }
  return menu_item;
}

GtkWidget *createMenubar() {
  const Menu file_open_child[] = {
      {"Open .uco", NULL, 0, onOpenUco},
      {"Open .lst", NULL, 0, onOpenLst},
  };
  const Menu file_save_child[] = {
      {"Save .uco", NULL, 0, onSaveUco},
      {"Save As .uco", NULL, 0, onSaveAsUco},
      {"Save .lst", NULL, 0, onSaveLst},
  };
  const Menu file_child[] = {
      {"Open", file_open_child, 2, NULL},
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
      {"Statistics", NULL, 0, onToggleStatistics},
  };
  const Menu help_child[] = {
      {"GUI 사용법", NULL, 0, NULL},
      {"About Team", NULL, 0, NULL},
  };
  const Menu menus[] = {
      {"File", file_child, 2, NULL},
      {"Run", run_child, 1, NULL},
      {"View", view_child, 6, NULL},
      {"Help", help_child, 2, NULL},
  };
  const int menu_count = sizeof(menus) / sizeof(menus[0]);

  GtkWidget *menubar = gtk_menu_bar_new();

  for (int i = 0; i < menu_count; i++) {
    GtkWidget *item = createMenuItem(&menus[i]);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item);
  }

  return menubar;
}
