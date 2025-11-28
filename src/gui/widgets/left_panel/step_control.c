#include "gui/gui_widgets.h"

#include "core/vm_context.h"

static GtkWidget *createControlBtns() {
  StepContext *step_ctx = &getGuiContext()->step_ctx;

  GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);

  GtkWidget *btn_first = gtk_button_new_with_label("◀◀ First");
  GtkWidget *btn_prev = gtk_button_new_with_label("◀ Prev");
  GtkWidget *btn_next = gtk_button_new_with_label("Next ▶");
  GtkWidget *btn_last = gtk_button_new_with_label("Last ▶▶");

  GtkWidget *btn_toggle = gtk_toggle_button_new();
  GtkWidget *icon = gtk_image_new_from_icon_name("media-playback-start",
                                                 GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(btn_toggle), icon);

  // 플레이 버튼 토클 이벤트
  g_signal_connect(btn_toggle, "toggled", G_CALLBACK(onIconToggled), icon);
  g_signal_connect(btn_toggle, "toggled", G_CALLBACK(onPlayToggled), NULL);

  // 버튼 클릭 이벤트
  g_signal_connect(btn_first, "clicked", G_CALLBACK(onFirstClicked), NULL);
  g_signal_connect(btn_prev, "clicked", G_CALLBACK(onPrevClicked), NULL);
  g_signal_connect(btn_next, "clicked", G_CALLBACK(onNextClicked), NULL);
  g_signal_connect(btn_last, "clicked", G_CALLBACK(onLastClicked), NULL);

  gtk_box_pack_start(GTK_BOX(button_box), btn_first, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), btn_prev, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), btn_toggle, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), btn_next, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(button_box), btn_last, FALSE, FALSE, 0);

  step_ctx->btn_first = btn_first;
  step_ctx->btn_prev = btn_prev;
  step_ctx->btn_next = btn_next;
  step_ctx->btn_last = btn_last;
  step_ctx->btn_toggle = btn_toggle;

  return button_box;
}

GtkWidget *createStepControl() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

  GtkAdjustment *adjustment = gtk_adjustment_new(0, 0, 0, 1, 10, 0);
  GtkWidget *scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
  gtk_scale_set_digits(GTK_SCALE(scale), 0);
  gtk_scale_set_draw_value(GTK_SCALE(scale), FALSE);

  GtkWidget *label = gtk_label_new("Step 00 of 00");

  // 스케일 변화 이벤트
  g_signal_connect(scale, "value-changed", G_CALLBACK(onScaleChanged), NULL);

  gtk_box_pack_start(GTK_BOX(box), scale, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), createControlBtns(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

  StepContext *step_ctx = &getGuiContext()->step_ctx;
  step_ctx->step_label = label;
  step_ctx->adj = adjustment;
  return box;
}

void initStep() {
  VMContext *vm_ctx = getVMContext();
  StepContext *step_ctx = &getGuiContext()->step_ctx;
  step_ctx->current_step = 0;

  gtk_adjustment_set_upper(step_ctx->adj, vm_ctx->changes.list_count - 1);
  gtk_adjustment_set_value(step_ctx->adj, 0);

  char buf[64];
  sprintf(buf, "Step 01 of %02d", vm_ctx->changes.list_count);
  gtk_label_set_text(step_ctx->step_label, buf);
}

void updateStep() {
  StepContext *step_ctx = &getGuiContext()->step_ctx;

  int value = gtk_adjustment_get_value(step_ctx->adj);
  int upper = gtk_adjustment_get_upper(step_ctx->adj);
  char buf[64];
  snprintf(buf, sizeof(buf), "Step %02d of %02d", value + 1, upper + 1);
  gtk_label_set_text(step_ctx->step_label, buf);

  step_ctx->current_step = value;
}
