#include "gui/gui.h"

#include "gui/gui_callbacks.h"
#include "io_utils/io_utils.h"

char *joinLines(char **lines, int line_count) {
  if (line_count == 0 || !lines)
    return NULL;

  // 전체 길이 계산
  size_t total_len = 0;
  for (int i = 0; i < line_count; i++) {
    if (lines[i]) {
      total_len += strlen(lines[i]) + 1; // 문자열 길이 + '\n'
    }
  }

  // 마지막 '\0' 포함
  char *result = malloc(total_len + 1);
  if (!result)
    return NULL;

  result[0] = '\0'; // 초기화

  // 문자열 합치기
  for (int i = 0; i < line_count; i++) {
    if (lines[i]) {
      strcat(result, lines[i]);
      strcat(result, "\n");
    }
  }

  return result;
}

static void loadLstToTextView(const char *filename) {
  // .lst 파일을 읽어서 GtkTextView에 표시
}

// 메뉴바의 "Open .uco" 메뉴 선택 시 호출되는 콜백
void onOpenUco() {
  GuiContext *ctx = getGuiContext();
  GtkWindow *parent = GTK_WINDOW(ctx->main_window);

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Open .uco File", parent, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel",
      GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.uco");
  gtk_file_filter_set_name(filter, "UCO files (*.uco)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if (filename) {
      ctx->current_step = 0;
      loadUcoToTable(filename);
      g_free(ctx->file_ctx.uco_filename);
      ctx->file_ctx.uco_filename = g_strdup(filename);
      g_free(filename);
    }
  }

  gtk_widget_destroy(dialog);
}

// 메뉴바의 "Open .lst" 메뉴 선택 시 호출되는 콜백
void onOpenLst() {
  GuiContext *ctx = getGuiContext();

  GtkTextView *lst_view = GTK_TEXT_VIEW(ctx->lst_view);
  GtkWindow *parent = GTK_WINDOW(ctx->main_window);

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Open .lst File", parent, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel",
      GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.lst");
  gtk_file_filter_set_name(filter, "LST files (*.lst)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if (filename) {
      loadLstToTextView(filename);
      g_free(filename);
    }
  }

  gtk_widget_destroy(dialog);
}

void onSaveUco() {
  GuiContext *ctx = getGuiContext();
  char *file_name = ctx->file_ctx.uco_filename;

  if (!file_name) {
    // 파일명이 없으면 Save As로 fallback
    onSaveAsUco();
    return;
  }

  char **lines = NULL;
  int line_count = 0;
  char *content;
  if (loadTableToUco(&lines, &line_count)) {
    content = joinLines(lines, line_count);
    for (int i = 0; i < line_count; i++) {
      free(lines[i]);
    }
    free(lines);
  }
  if (!saveUco(file_name, content)) {
    g_warning("Failed to save .uco file: %s", file_name);
  }
  g_free(content);
}

static char *ensureUcoExtension(const char *filename) {
  const char *ext = strrchr(filename, '.');
  if (!ext) {
    // 확장자가 없으면 .uco 붙임
    return g_strdup_printf("%s.uco", filename);
  }
  return g_strdup(filename); // 이미 확장자가 있으면 그대로
}

void onSaveAsUco() {
  GuiContext *ctx = getGuiContext();
  GtkWindow *parent = GTK_WINDOW(ctx->main_window);

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Save .uco File", parent, GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel",
      GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.uco");
  gtk_file_filter_set_name(filter, "UCO files (*.uco)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if (filename) {
      char *final_name = ensureUcoExtension(filename);

      char **lines = NULL;
      int line_count = 0;
      char *content;
      if (loadTableToUco(&lines, &line_count)) {
        content = joinLines(lines, line_count);
        for (int i = 0; i < line_count; i++) {
          free(lines[i]);
        }
        free(lines);
      }
      if (!saveUco(final_name, content)) {
        g_warning("Failed to save .uco file: %s", final_name);
      } else {
        g_free(ctx->file_ctx.uco_filename);
        ctx->file_ctx.uco_filename = g_strdup(final_name);
      }
      g_free(content);
      g_free(filename);
    }
  }

  gtk_widget_destroy(dialog);
}

void onSaveLst() {
  GuiContext *ctx = getGuiContext();
  GtkWindow *parent = GTK_WINDOW(ctx->main_window);

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Save .lst File", parent, GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel",
      GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.lst");
  gtk_file_filter_set_name(filter, "LST files (*.lst)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = ctx->file_ctx.lst_filename;
    if (filename) {
      // load 하기
      // save 하기
      // g_free(content);
      g_free(filename);
    }
  }

  gtk_widget_destroy(dialog);
}