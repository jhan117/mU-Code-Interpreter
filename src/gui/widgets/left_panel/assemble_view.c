#include "gui/gui.h"

#include "assemble.h"
#include "core/vm_context.h"

void setAssembleView();

// 어셈블 화면 초기화
TextScrollInfo initAssembleView() {
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

void setAssembleView() {
  VMContext *ctx = getVMContext();

  GtkTextBuffer *buffer =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(getGuiContext()->assemble_view));
  gtk_text_buffer_set_text(buffer, "", -1);

  char line[128];
  for (int i = 0; i < ctx->code_len; i++) {
    int inst = ctx->memory[i];
    int opGroup = (inst >> 29) & 0x7;
    int opGroupIdx = (inst >> 26) & 0x7;
    int opcode = opGroup * 10 + opGroupIdx;
    int operand = inst & 0x03FFFFFF;

    if (operand & (1 << 25))
      operand |= ~0x03FFFFFF;

    const OpInfo *info = findByCode(opcode);

    if (info->operand_count == 0) {
      if (i == ctx->code_len - 1) {
        snprintf(line, sizeof(line), "%04d: operator=%d%d", i + 1, opGroup,
                 opGroupIdx);
      } else {
        snprintf(line, sizeof(line), "%04d: operator=%d%d\n", i + 1, opGroup,
                 opGroupIdx);
      }
    } else {
      if (i == ctx->code_len - 1) {
        snprintf(line, sizeof(line), "%04d: operator=%d%d operand=%d", i + 1,
                 opGroup, opGroupIdx, operand);
      } else {
        snprintf(line, sizeof(line), "%04d: operator=%d%d operand=%d\n", i + 1,
                 opGroup, opGroupIdx, operand);
      }
    }

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, line, -1);
  }
}