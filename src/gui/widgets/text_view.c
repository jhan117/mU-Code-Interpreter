#include "gui/gui.h"

#include "core/vm_context.h"

void showAssembleResult(GtkWidget *text_view) {
  VMContext *ctx = getVMContext();

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  gtk_text_buffer_set_text(buffer, "", -1);

  // 예시: 테스트용 메모리 초기화
  for (int i = 0; i < INIT_MEMORY_SIZE; i++) {
    ctx->memory[i] = 0; // 기본값
  }

  // 몇 개만 임의로 채워보기
  ctx->memory[0] = (5 << 26) | 123; // opcode=5, operand=123
  ctx->memory[1] =
      (12 << 26) | (1 << 25); // opcode=12, operand=-33554432 (signed 변환됨)
  ctx->memory[2] = 0;         // 종료 조건

  for (int i = 0; i < INIT_MEMORY_SIZE; i++) {
    int inst = ctx->memory[i];
    int opcode = (inst >> 26) & 0x3F;
    int operand = inst & 0x03FFFFFF;

    if (operand & (1 << 25))
      operand |= ~0x03FFFFFF;

    if (opcode == 0 && operand == 0)
      break;

    char line[128];
    snprintf(line, sizeof(line), "%04d: opcode=%02d operand=%d\n", i, opcode,
             operand);

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, line, -1);
  }
}

char *getTextFromView(GtkTextView *view) {
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);

  return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

// 한줄씩 채우긴 하는데... 흠 일단 만들어놓고 나중에 생각
void setTextToView(GtkTextView *view, const char *text) {
  if (!view || !text)
    return;

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
  gtk_text_buffer_set_text(buffer, text, -1);
}

TextScrollInfo createTextView() {

  GtkWidget *text_view = gtk_text_view_new();
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 12);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 8);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 8);

  GtkWidget *text_scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(text_scroll), text_view);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(text_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  TextScrollInfo info = {text_view, text_scroll};
  return info;
}
