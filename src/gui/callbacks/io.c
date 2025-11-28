#include "gui/gui_callbacks.h"

#include "assembler/assemble_utils.h" // isNumber()

static void handleLineInput(IOContext *io_ctx, const gchar *line) {
  gchar *trimmed = g_strstrip((gchar *)line);
  if (isNumber(trimmed) && strlen(trimmed) > 0) {
    int val = atoi(trimmed);
    g_async_queue_push(io_ctx->input_queue, GINT_TO_POINTER(val));
    g_print("숫자 입력됨: %d\n", val);
  } else if (strlen(trimmed) > 0) {
    g_print("숫자가 아님, 무시됨: %s\n", trimmed);
  }
}

// 붙여넣기/입력 텍스트 처리
static void processInput(IOContext *io_ctx, const gchar *text) {
  gchar **lines = g_strsplit(text, "\n", 0);
  for (int i = 0; lines[i] != NULL; i++) {
    handleLineInput(io_ctx, lines[i]);
  }
  g_strfreev(lines);
}

void onInsertText(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text,
                  gint len, gpointer user_data) {

  IOContext *io_ctx = (IOContext *)user_data;
  const gchar *prefix = ">> ";
  GtkTextIter end_iter;
  gtk_text_buffer_get_end_iter(buffer, &end_iter);

  // 마지막 줄 시작
  int last_line = gtk_text_buffer_get_line_count(buffer) - 1;
  GtkTextIter last_line_start;
  gtk_text_buffer_get_iter_at_line(buffer, &last_line_start, last_line);

  // 마지막 줄이 아니면 입력 차단
  if (gtk_text_iter_compare(location, &last_line_start) < 0) {
    g_signal_stop_emission_by_name(buffer, "insert-text");
    return;
  }

  // Enter 또는 붙여넣기 처리
  if (strchr(text, '\n') != NULL) {
    g_signal_stop_emission_by_name(buffer, "insert-text");

    // 프롬프트 제외 문자열만 가져오기
    GtkTextIter line_start = last_line_start;
    gtk_text_iter_forward_chars(&line_start, 3); // >> 길이만큼
    gchar *line_text =
        gtk_text_buffer_get_text(buffer, &line_start, &end_iter, FALSE);
    if (line_text) {
      processInput(io_ctx, line_text); // 숫자만 큐에 push
      g_free(line_text);
    }

    // 새 줄 + 프롬프트
    insertAtEnd(io_ctx, "\n>> ");
    return;
  }

  // 커서 항상 마지막 위치
  gtk_text_buffer_get_end_iter(buffer, &end_iter);
  gtk_text_buffer_place_cursor(buffer, &end_iter);

  if (gtk_text_iter_get_line_offset(location) == 0) {
    gtk_text_buffer_insert(buffer, location, prefix, -1);
  } else {
    gtk_text_buffer_insert(buffer, location, text, len);
  }
}

void guiIoWrite(const char *user_data) {
  IOContext *io_ctx = (IOContext *)user_data;

  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(io_ctx->io_view));

  int last_line_num = gtk_text_buffer_get_line_count(buf) - 1;
  if (last_line_num < 0)
    last_line_num = 0;

  GtkTextIter last_line_start;
  gtk_text_buffer_get_iter_at_line(buf, &last_line_start, last_line_num);
  GtkTextIter end_iter;
  gtk_text_buffer_get_end_iter(buf, &end_iter);

  // 마지막 줄 처음 3글자 확인
  gchar *first3 =
      gtk_text_buffer_get_text(buf, &last_line_start, &end_iter, FALSE);
  gboolean last_is_input = (first3 && g_strcmp0(first3, ">> ") != 0);
  if (first3)
    g_free(first3);

  char tmp[128];
  if (last_is_input)
    snprintf(tmp, sizeof(tmp), "%s ", user_data); // 연속 출력 공백 이어붙이기
  else
    snprintf(tmp, sizeof(tmp), "\n<< %s ", user_data); // 새 줄 + << 표시

  insertAtEnd(io_ctx, tmp);
}

int guiIoRead() {
  IOContext *io_ctx = &getGuiContext()->io_ctx;

  return GPOINTER_TO_INT(g_async_queue_pop(io_ctx->input_queue));
}

void guiIoLf() {
  GtkWidget *io_view = getGuiContext()->io_ctx.io_view;
  insertAtEnd(io_view, "\n");
}