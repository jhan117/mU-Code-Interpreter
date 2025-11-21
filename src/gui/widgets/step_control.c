#include "gui/gui.h"

static void onToggled(GtkToggleButton *button, gpointer user_data) {
  GuiContext *ctx = getGuiContext();
  GtkImage *icon = GTK_IMAGE(user_data);

  if (gtk_toggle_button_get_active(button)) {
    gtk_image_set_from_icon_name(icon, "media-playback-pause",
                                 GTK_ICON_SIZE_BUTTON);
    gtk_widget_set_sensitive(ctx->btn_first, FALSE);
    gtk_widget_set_sensitive(ctx->btn_prev, FALSE);
    gtk_widget_set_sensitive(ctx->btn_next, FALSE);
    gtk_widget_set_sensitive(ctx->btn_last, FALSE);
  } else {
    gtk_image_set_from_icon_name(icon, "media-playback-start",
                                 GTK_ICON_SIZE_BUTTON);
    gtk_widget_set_sensitive(ctx->btn_first, TRUE);
    gtk_widget_set_sensitive(ctx->btn_prev, TRUE);
    gtk_widget_set_sensitive(ctx->btn_next, TRUE);
    gtk_widget_set_sensitive(ctx->btn_last, TRUE);
  }
}

static GtkWidget *createPlayControl(GtkAdjustment *adj, GtkLabel *label) {
  GuiContext *ctx = getGuiContext();

  GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);

  ctx->btn_first = gtk_button_new_with_label("◀◀ First");
  ctx->btn_prev = gtk_button_new_with_label("◀ Prev");
  ctx->btn_next = gtk_button_new_with_label("Next ▶");
  ctx->btn_last = gtk_button_new_with_label("Last ▶▶");

  ctx->btn_toggle = gtk_toggle_button_new();
  GtkWidget *icon = gtk_image_new_from_icon_name("media-playback-start",
                                                 GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(ctx->btn_toggle), icon);
  g_signal_connect(ctx->btn_toggle, "toggled", G_CALLBACK(onToggled), icon);

  g_signal_connect(ctx->btn_first, "clicked", G_CALLBACK(onFirstClicked), adj);
  g_signal_connect(ctx->btn_prev, "clicked", G_CALLBACK(onPrevClicked), adj);
  g_signal_connect(ctx->btn_next, "clicked", G_CALLBACK(onNextClicked), adj);
  g_signal_connect(ctx->btn_last, "clicked", G_CALLBACK(onLastClicked), adj);
  g_signal_connect(ctx->btn_toggle, "toggled", G_CALLBACK(onPlayToggled), adj);

  gtk_box_pack_start(GTK_BOX(button_box), ctx->btn_first, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), ctx->btn_prev, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), ctx->btn_toggle, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), ctx->btn_next, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), ctx->btn_last, FALSE, FALSE, 0);

  return button_box;
}

GtkWidget *createStepControl() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  // 최소값 0, 최대값 100, 초기값 0
  GtkAdjustment *adjustment = gtk_adjustment_new(0, 0, 100, 1, 10, 0);
  GtkWidget *scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
  gtk_scale_set_digits(GTK_SCALE(scale), 0);
  gtk_scale_set_draw_value(GTK_SCALE(scale), TRUE);

  GtkWidget *label = gtk_label_new("Step 00 of 00");

  g_signal_connect(scale, "value-changed", G_CALLBACK(onScaleChanged), label);

  gtk_box_pack_start(GTK_BOX(box), scale, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box),
                     createPlayControl(adjustment, GTK_LABEL(label)), FALSE,
                     FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

  return box;
}