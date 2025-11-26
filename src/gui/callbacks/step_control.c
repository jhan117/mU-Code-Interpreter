#include "gui/gui_callbacks.h"

static int onPlayStep(gpointer user_data) {
  StepContext *step_ctx = &getGuiContext()->step_ctx;

  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);
  int val = gtk_adjustment_get_value(adj);
  int upper = gtk_adjustment_get_upper(adj);

  if (val < upper) {
    gtk_adjustment_set_value(adj, val + 1);
    return 1;
  } else {
    step_ctx->timeout_id = 0;
    return 0;
  }
}

// Play/Pause 토글 콜백
void onPlayToggled(GtkToggleButton *button) {
  StepContext *step_ctx = &getGuiContext()->step_ctx;

  if (gtk_toggle_button_get_active(button)) {
    if (step_ctx->timeout_id == 0) {
      step_ctx->timeout_id =
          g_timeout_add(500, onPlayStep, step_ctx->adj); // 0.5초마다 Step 증가
    }
    toggleWidgetsStatus(0);
  } else {
    if (step_ctx->timeout_id != 0) {
      g_source_remove(step_ctx->timeout_id);
      step_ctx->timeout_id = 0;
    }
    toggleWidgetsStatus(1);
  }
}

// Scale 값 변경 시 호출되는 콜백
void onScaleChanged(GtkRange *range) { updateStep(); }

// 버튼 콜백들
void onFirstClicked(GtkButton *button) {
  GtkAdjustment *adj = getGuiContext()->step_ctx.adj;
  gtk_adjustment_set_value(adj, gtk_adjustment_get_lower(adj));
}
void onPrevClicked(GtkButton *button) {
  GtkAdjustment *adj = getGuiContext()->step_ctx.adj;
  gtk_adjustment_set_value(adj, gtk_adjustment_get_value(adj) - 1);
}
void onNextClicked(GtkButton *button) {
  GtkAdjustment *adj = getGuiContext()->step_ctx.adj;
  gtk_adjustment_set_value(adj, gtk_adjustment_get_value(adj) + 1);
}
void onLastClicked(GtkButton *button) {
  GtkAdjustment *adj = getGuiContext()->step_ctx.adj;
  gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
}

// 아이콘 토글
void onIconToggled(GtkToggleButton *button, gpointer user_data) {
  StepContext *step_ctx = &getGuiContext()->step_ctx;
  GtkImage *icon = GTK_IMAGE(user_data);

  if (gtk_toggle_button_get_active(button)) {
    gtk_image_set_from_icon_name(icon, "media-playback-pause",
                                 GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_icon_name(icon, "media-playback-start",
                                 GTK_ICON_SIZE_BUTTON);
  }
}