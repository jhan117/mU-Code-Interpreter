#include "gui/callbacks.h"

#include "gui/gui_context.h"

gboolean onEnterPress(GtkWidget *widget, GdkEventKey *event,
                      gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  if (event->keyval == GDK_KEY_Return) {
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
    GtkTextIter start, end;

    // 현재 줄 가져오기
    gtk_text_buffer_get_end_iter(buf, &end);
    gtk_text_iter_set_line_offset(&end, 0); // 줄 시작
    start = end;
    gtk_text_iter_forward_to_line_end(&end);

    char *line = gtk_text_buffer_get_text(buf, &start, &end, FALSE);
    if (g_str_has_prefix(line, ">> ")) {
      const char *input = line + 3;
      if (*input != '\0') {
        char *endptr;
        long val = strtol(input, &endptr, 10);
        if (endptr != input && *endptr == '\0') {
          ctx->value = (int)val;
          ctx->got_input = 1; // 입력 완료 표시
        } else {
          // 잘못된 입력이면 에러 메시지 출력
          GtkTextIter buf_end;
          gtk_text_buffer_get_end_iter(buf, &buf_end);
          gtk_text_buffer_insert(buf, &buf_end, "\n<< Error: 숫자만 입력하세요",
                                 -1);
          gtk_text_buffer_insert(buf, &buf_end, "\n>> ", -1);
        }
      }
    }
    g_free(line);
    return TRUE; // 기본 엔터 동작 막기
  }
  return FALSE;
}

void guiIoWrite(int data) {
  GtkTextBuffer *buf =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(getGuiContext()->io_view));
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buf, &end);

  char tmp[32];
  sprintf(tmp, "%d ", data); // 숫자 뒤에 공백 추가

  if (gtk_text_buffer_get_char_count(buf) == 0) {
    gtk_text_buffer_insert(buf, &end, "<< ", -1);
  } else {
    gtk_text_buffer_insert(buf, &end, "\n<< ", -1);
  }
  gtk_text_buffer_insert(buf, &end, tmp, -1);
}

int guiIoRead() {
  GuiContext *ctx = getGuiContext();
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ctx->io_view));
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buf, &end);

  // 프롬프트 출력
  if (gtk_text_buffer_get_char_count(buf) == 0) {
    gtk_text_buffer_insert(buf, &end, ">> ", -1);
  } else {
    gtk_text_buffer_insert(buf, &end, "\n>> ", -1);
  }

  // 엔터 이벤트 기다리기
  ctx->got_input = 0;
  while (!ctx->got_input) {
    while (gtk_events_pending())
      gtk_main_iteration();
  }

  return ctx->value;
}

void guiIoLf() {
  GtkTextBuffer *buf =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(getGuiContext()->io_view));
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buf, &end);

  gtk_text_buffer_insert(buf, &end, "\n", -1);
}