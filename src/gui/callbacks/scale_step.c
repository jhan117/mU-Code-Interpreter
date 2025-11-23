#include "gui/gui.h"

#include "core/vm_context.h"

gboolean onPlayStep(gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);
  double val = gtk_adjustment_get_value(adj);
  double upper = gtk_adjustment_get_upper(adj);

  if (val < upper) {
    gtk_adjustment_set_value(adj, val + 1);
    return TRUE; // 계속 반복
  } else {
    ctx->timeout_id = 0; // 끝까지 갔으면 타이머 종료
    return FALSE;
  }
}

// Play/Pause 토글 콜백
void onPlayToggled(GtkToggleButton *button, gpointer user_data) {
  GuiContext *ctx = getGuiContext();
  GtkAdjustment *adj = GTK_ADJUSTMENT(user_data);

  if (gtk_toggle_button_get_active(button)) {
    // Play 시작
    if (ctx->timeout_id == 0) {
      ctx->timeout_id =
          g_timeout_add(500, onPlayStep, adj); // 0.5초마다 Step 증가
    }
  } else {
    // Pause → 타이머 제거
    if (ctx->timeout_id != 0) {
      g_source_remove(ctx->timeout_id);
      ctx->timeout_id = 0;
    }
  }
}

// 라벨 업데이트 함수
void updateLabel(GtkLabel *label, GtkAdjustment *adjustment) {
  int value = (int)gtk_adjustment_get_value(adjustment);
  int upper = (int)gtk_adjustment_get_upper(adjustment);
  char buf[64];
  snprintf(buf, sizeof(buf), "Step %02d of %02d", value + 1, upper + 1);
  gtk_label_set_text(label, buf);
}

void highlightLine(GtkTextView *view, int line_number) {
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

void highlightRow() {
  GuiContext *g_ctx = getGuiContext();
  VMContext *vm_ctx = getVMContext();
  GtkListStore *store = g_ctx->ucode_table->list_data;
  int prev = g_ctx->uco_prev_line;
  int cur = vm_ctx->source_map.line[g_ctx->current_step];

  GtkTreeIter iter;

  // 이전 강조 제거
  if (prev >= 0 &&
      gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, prev)) {
    gtk_list_store_set(store, &iter, 3, NULL, -1); // 기본 배경
  }

  // 현재 강조 설정
  if (cur >= 0 &&
      gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, cur)) {
    gtk_list_store_set(store, &iter, 3, "#ff0000", -1);
  }

  g_ctx->uco_prev_line = cur;
}

// Scale 값 변경 시 호출되는 콜백
void onScaleChanged(GtkRange *range, gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  GtkLabel *label = GTK_LABEL(user_data);
  GtkAdjustment *adj = gtk_range_get_adjustment(range);
  ctx->current_step = gtk_adjustment_get_value(adj);
  updateLabel(label, adj);

  highlightLine(GTK_TEXT_VIEW(ctx->assemble_view), gtk_range_get_value(range));
  highlightRow();

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