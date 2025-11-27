#include "gui/gui_callbacks.h"

void onToggleRegisters(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.reg_view);
  else
    gtk_widget_hide(status_ctx.reg_view);
}

void onToggleCPU(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.cpu_view);
  else
    gtk_widget_hide(status_ctx.cpu_view);
}

void onToggleMemory(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.memory_view);
  else
    gtk_widget_hide(status_ctx.memory_view);
}

void onToggleLabels(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.labels_view);
  else
    gtk_widget_hide(status_ctx.labels_view);
}

void onToggleSymbols(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.symbols_view);
  else
    gtk_widget_hide(status_ctx.symbols_view);
}
