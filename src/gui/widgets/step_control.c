#include "gui.h"

static void onToggled(GtkToggleButton *button, gpointer user_data) {
  GtkImage *icon = GTK_IMAGE(user_data);

  if (gtk_toggle_button_get_active(button)) {
    gtk_image_set_from_icon_name(icon, "media-playback-pause",
                                 GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_icon_name(icon, "media-playback-start",
                                 GTK_ICON_SIZE_BUTTON);
  }
}

GtkWidget *createStepControl() {
  // TODO: 나중에 버튼 생성 부분을 함수로 분리하기
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  // 최소값 0, 최대값 100, 초기값 0
  GtkAdjustment *adjustment = gtk_adjustment_new(0, 0, 100, 1, 10, 0);

  GtkWidget *scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
  gtk_scale_set_digits(GTK_SCALE(scale), 0);
  gtk_scale_set_draw_value(GTK_SCALE(scale), TRUE);

  GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  GtkWidget *label = gtk_label_new("Step 00 of 00");
  GtkWidget *button_first = gtk_button_new_with_label("<< first");
  GtkWidget *button_prev = gtk_button_new_with_label("< prev");
  GtkWidget *toggle = gtk_toggle_button_new();
  GtkWidget *icon = gtk_image_new_from_icon_name("media-playback-start",
                                                 GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(toggle), icon);
  g_signal_connect(toggle, "toggled", G_CALLBACK(onToggled), icon);
  GtkWidget *button_next = gtk_button_new_with_label("next >");
  GtkWidget *button_last = gtk_button_new_with_label("last >>");

  gtk_box_pack_start(GTK_BOX(button_box), button_first, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), button_prev, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), toggle, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), button_next, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), button_last, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(box), scale, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  return box;
}