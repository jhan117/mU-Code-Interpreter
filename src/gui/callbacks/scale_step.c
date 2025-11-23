#include "gui/gui.h"

gboolean onPlayStep(gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);
  double val = gtk_adjustment_get_value(adj);
  double upper = gtk_adjustment_get_upper(adj);

  if (val < upper) {
    gtk_adjustment_set_value(adj, val + 1);
    return TRUE; // 계속 반복
  } else {
    ctx->current_step = 0; // 끝까지 갔으면 타이머 종료
    return FALSE;
  }
}

// Play/Pause 토글 콜백
void onPlayToggled(GtkToggleButton *button, gpointer user_data) {
  GuiContext *ctx = getGuiContext();
  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);

  if (gtk_toggle_button_get_active(button)) {
    // Play 시작
    if (ctx->current_step == 0) {
      ctx->current_step =
          g_timeout_add(500, onPlayStep, adj); // 0.5초마다 Step 증가
    }
  } else {
    // Pause → 타이머 제거
    if (ctx->current_step != 0) {
      g_source_remove(ctx->current_step);
      ctx->current_step = 0;
    }
  }
}

// 라벨 업데이트 함수
void updateLabel(GtkLabel *label, GtkAdjustment *adjustment) {
  int value = (int)gtk_adjustment_get_value(adjustment);
  int upper = (int)gtk_adjustment_get_upper(adjustment);
  char buf[64];
  snprintf(buf, sizeof(buf), "Step %02d of %02d", value, upper);
  gtk_label_set_text(label, buf);
}

static void highlightLine(GtkTextView *view, int line_number) {
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);

  // 태그 준비
  GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
  GtkTextTag *tag = gtk_text_tag_table_lookup(table, "highlight");
  if (!tag) {
    tag = gtk_text_tag_new("highlight");
    g_object_set(tag, "foreground", "red", NULL);
    gtk_text_tag_table_add(table, tag);
  }

  // 모든 텍스트에서 기존 태그 제거
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_remove_tag(buffer, tag, &start, &end);

  // 지정된 줄에 태그 적용
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_line(buffer, &iter, line_number);

  GtkTextIter line_end = iter;
  gtk_text_iter_forward_to_line_end(&line_end);

  gtk_text_buffer_apply_tag(buffer, tag, &iter, &line_end);
}

// Scale 값 변경 시 호출되는 콜백
void onScaleChanged(GtkRange *range, gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  GtkLabel *label = GTK_LABEL(user_data);
  GtkAdjustment *adj = gtk_range_get_adjustment(range);
  ctx->current_step = gtk_adjustment_get_value(adj);
  updateLabel(label, adj);

  highlightLine(GTK_TEXT_VIEW(ctx->assemble_view), gtk_range_get_value(range));

  updateVM(ctx->current_step);
}

// 버튼 콜백들
void onFirstClicked(GtkButton *button, gpointer user_data) {
  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);
  gtk_adjustment_set_value(adj, gtk_adjustment_get_lower(adj));
}

void onPrevClicked(GtkButton *button, gpointer user_data) {
  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);
  double val = gtk_adjustment_get_value(adj);
  gtk_adjustment_set_value(adj, val - 1);
}

void onNextClicked(GtkButton *button, gpointer user_data) {
  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);
  double val = gtk_adjustment_get_value(adj);
  gtk_adjustment_set_value(adj, val + 1);
}

void onLastClicked(GtkButton *button, gpointer user_data) {
  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);
  gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
}