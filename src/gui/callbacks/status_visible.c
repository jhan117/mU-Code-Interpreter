#include "gui/gui_callbacks.h"

static void updateStatusVisibility() {
  GuiContext *ctx = getGuiContext();
  gboolean visible = FALSE;

  if (gtk_widget_get_visible(ctx->status_ctx.reg_view))
    visible = TRUE;
  if (gtk_widget_get_visible(ctx->status_ctx.cpu_view))
    visible = TRUE;
  if (gtk_widget_get_visible(ctx->status_ctx.memory_view))
    visible = TRUE;
  if (gtk_widget_get_visible(ctx->status_ctx.labels_view))
    visible = TRUE;
  if (gtk_widget_get_visible(ctx->status_ctx.symbols_view))
    visible = TRUE;

  if (visible)
    gtk_widget_show(ctx->status_ctx.status_box);
  else
    gtk_widget_hide(ctx->status_ctx.status_box);
}

void onToggleRegisters(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.reg_view);
  else
    gtk_widget_hide(status_ctx.reg_view);

  updateStatusVisibility();
}

void onToggleCPU(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.cpu_view);
  else
    gtk_widget_hide(status_ctx.cpu_view);

  updateStatusVisibility();
}

void onToggleMemory(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.memory_view);
  else
    gtk_widget_hide(status_ctx.memory_view);

  updateStatusVisibility();
}

void onToggleLabels(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.labels_view);
  else
    gtk_widget_hide(status_ctx.labels_view);

  updateStatusVisibility();
}

void onToggleSymbols(GtkWidget *widget, gpointer user_data) {
  StatusContext status_ctx = getGuiContext()->status_ctx;
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if (active)
    gtk_widget_show(status_ctx.symbols_view);
  else
    gtk_widget_hide(status_ctx.symbols_view);

  updateStatusVisibility();
}

void onToggleStatistics(GtkWidget *widget, gpointer user_data) {
  // StatusContext *status_ctx = getGuiContext()->status_ctx;
  // gboolean active =
  // gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  // if (active)
  //   gtk_widget_show(status_ctx.cpu_view);
  // else
  //   gtk_widget_hide(status_ctx.cpu_view);

  // updateStatusVisibility();
}
