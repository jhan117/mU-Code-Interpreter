#include "gui/gui.h"

#include "io_utils/io_utils.h"

// 파일 읽기 함수 통합할까나...
static void loadUcoToTextView(const char *filename) {
  GuiContext *ctx = getGuiContext();
  GtkTextView *view = GTK_TEXT_VIEW(ctx->ucode_view);
  ctx->current_step = 0;

  char **lines = NULL;
  int line_count = 0;

  if (loadUco(filename, &lines, &line_count)) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
    gtk_text_buffer_set_text(buffer, "", -1);

    for (int i = 0; i < line_count; i++) {
      gtk_text_buffer_insert_at_cursor(buffer, lines[i], -1);
      gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
    }

    freeUco(lines, line_count);
  }
}

static void loadLstToTextView(const char *filename) {
  // .lst 파일을 읽어서 GtkTextView에 표시
}

// file_chooser 버튼에서 파일 선택 시 호출되는 콜백
void onFileChosen(GtkFileChooserButton *chooser) {
  char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
  if (!filename)
    return;

  loadUcoToTextView(filename);
  g_free(filename);
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
      loadUcoToTextView(filename);
      g_free(ctx->uco_filename);
      ctx->uco_filename = g_strdup(filename);
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

void onSaveUco(GtkMenuItem *item) {
  GuiContext *ctx = getGuiContext();
  char *file_name = ctx->uco_filename;

  if (!file_name) {
    // 파일명이 없으면 Save As로 fallback
    onSaveAsUco();
    return;
  }

  char *content = getTextFromView(GTK_TEXT_VIEW(ctx->ucode_view));
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

      char *content = getTextFromView(GTK_TEXT_VIEW(ctx->ucode_view));
      if (!saveUco(final_name, content)) {
        g_warning("Failed to save .uco file: %s", final_name);
      } else {
        g_free(ctx->uco_filename);
        ctx->uco_filename = g_strdup(final_name); // 새 파일명 저장
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
    char *filename = ctx->lst_filename;
    if (filename) {
      char *content = getTextFromView(GTK_TEXT_VIEW(ctx->lst_view));
      // save 하기
      g_free(content);
      g_free(filename);
    }
  }

  gtk_widget_destroy(dialog);
}