#include "gui/gui_callbacks.h"

void updateStatusBox() {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  GtkWidget *status_box = status_ctx.status_box; // 이미 저장돼 있다고 가정

  gboolean any_visible = FALSE;

  if (gtk_widget_get_visible(status_ctx.cpu_view))
    any_visible = TRUE;
  if (gtk_widget_get_visible(status_ctx.memory_view))
    any_visible = TRUE;
  if (gtk_widget_get_visible(status_ctx.labels_view))
    any_visible = TRUE;
  if (gtk_widget_get_visible(status_ctx.symbols_view))
    any_visible = TRUE;

  if (any_visible)
    gtk_widget_show(status_box);
  else
    gtk_widget_hide(status_box);
}

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
  updateStatusBox();
}

void onToggleMemory(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.memory_view);
  else
    gtk_widget_hide(status_ctx.memory_view);

  updateStatusBox();
}

void onToggleLabels(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.labels_view);
  else
    gtk_widget_hide(status_ctx.labels_view);

  updateStatusBox();
}

void onToggleSymbols(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.symbols_view);
  else
    gtk_widget_hide(status_ctx.symbols_view);

  updateStatusBox();
}
