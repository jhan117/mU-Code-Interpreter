#include "gui/gui_callbacks.h"

#include "io_utils/io_utils.h"

void onFileChosen(GtkFileChooserButton *chooser) {
  FileContext *file_ctx = &getGuiContext()->file_ctx;

  char *filename = gtk_file_chooser_get_filename(chooser);
  if (!filename)
    return;

  g_free(file_ctx->uco_filename);
  file_ctx->uco_filename = g_strdup(filename);

  char **lines = NULL;
  int line_count = 0;
  if (!loadUco(filename, &lines, &line_count)) {
    showMessage(GTK_MESSAGE_ERROR, ".uco 파일을 불러 올 수 없습니다.");
    g_free(filename);
    return;
  }

  char *content = joinLines(lines, line_count);
  updateUcodeView(content);
  freeUco(lines, line_count);
  g_free(filename);
}

static char *makeFilename(const char *filename, const char *extension) {
  const char *ext = strrchr(filename, '.');
  if (!ext) {
    // 확장자가 없으면 .uco 붙임
    return g_strdup_printf("%s.%s", filename, extension);
  }
  return g_strdup(filename);
}

// 메뉴바의 "Open .uco" 메뉴 선택 시 호출되는 콜백
void onOpenUco() {
  GuiContext *ctx = getGuiContext();

  if (ctx->is_run) {
    showMessage(GTK_MESSAGE_WARNING, "실행 중입니다. 나중에 시도하세요.");
    return;
  }

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Open .uco File", ctx->main_window, GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.uco");
  gtk_file_filter_set_name(filter, "UCO files (*.uco)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if (filename) {
      char **lines = NULL;
      int line_count = 0;
      if (!loadUco(filename, &lines, &line_count)) {
        showMessage(GTK_MESSAGE_ERROR, ".uco 파일을 불러 올 수 없습니다.");
        g_free(filename);
        return;
      }

      char *content = joinLines(lines, line_count);
      updateUcodeView(content);
      freeUco(lines, line_count);
      g_free(ctx->file_ctx.uco_filename);
      ctx->file_ctx.uco_filename = g_strdup(filename);
      ctx->step_ctx.current_step = 0;
    }
  }

  gtk_widget_destroy(dialog);
}

void onSaveUco() {
  GuiContext *ctx = getGuiContext();
  if (ctx->is_run) {
    showMessage(GTK_MESSAGE_WARNING, "실행 중입니다. 나중에 시도하세요.");
    return;
  }

  char *file_name = ctx->file_ctx.uco_filename;

  if (!file_name) {
    // 파일명이 없으면 Save As로 fallback
    onSaveAsUco();
    return;
  }

  char **lines = NULL;
  int line_count = 0;
  if (getUcodeView(&lines, &line_count)) {
    if (!saveUco(file_name, lines, line_count)) {
      showMessage(GTK_MESSAGE_ERROR, ".uco 파일을 저장할 수 없습니다.");
    } else {
      showMessage(GTK_MESSAGE_INFO, ".uco 파일이 저장되었습니다.");
    }
  }
}

void onSaveAsUco() {
  GuiContext *ctx = getGuiContext();

  if (ctx->is_run) {
    showMessage(GTK_MESSAGE_WARNING, "실행 중입니다. 나중에 시도하세요.");
    return;
  }

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Save .uco File", ctx->main_window, GTK_FILE_CHOOSER_ACTION_SAVE,
      "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.uco");
  gtk_file_filter_set_name(filter, "UCO files (*.uco)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if (filename) {
      char *final_name = makeFilename(filename, "uco");
      char **lines = NULL;
      int line_count = 0;
      if (getUcodeView(&lines, &line_count)) {
        if (!saveUco(final_name, lines, line_count)) {
          showMessage(GTK_MESSAGE_ERROR, ".uco 파일을 저장할 수 없습니다.");
        } else {
          showMessage(GTK_MESSAGE_INFO, ".uco 파일이 저장되었습니다.");
          g_free(ctx->file_ctx.uco_filename);
          ctx->file_ctx.uco_filename = g_strdup(final_name);
        }
      }

      g_free(final_name);
      g_free(filename);
    }
  }

  gtk_widget_destroy(dialog);
}

static char *makeLstFilename(const char *uco_filename,
                             const char *lst_filename) {
  if (lst_filename) {
    return g_strdup(lst_filename);
  }

  if (uco_filename) {
    char *base = g_path_get_basename(uco_filename); // ex4.uco
    char *dot = strrchr(base, '.');                 // 마지막 '.' 찾기
    char *result;

    if (dot) {
      *dot = '\0'; // 확장자 제거
    }

    result = g_strdup_printf("%s.lst", base);
    g_free(base);
    return result;
  }

  return NULL;
}

void onSaveLst() {
  GuiContext *ctx = getGuiContext();

  if (ctx->is_run) {
    showMessage(GTK_MESSAGE_WARNING, "실행 중입니다. 나중에 시도하세요.");
    return;
  }

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Save .lst File", ctx->main_window, GTK_FILE_CHOOSER_ACTION_SAVE,
      "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.lst");
  gtk_file_filter_set_name(filter, "LST files (*.lst)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  char *default_name = NULL;
  if (ctx->file_ctx.lst_filename) {
    char *base = g_path_get_basename(ctx->file_ctx.lst_filename);
    default_name = g_strdup(base);
    g_free(base);
  } else {
    default_name = makeLstFilename(ctx->file_ctx.uco_filename, NULL);
  }

  if (default_name) {
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), default_name);
    g_free(default_name);
  }

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if (filename) {
      char *final_name = makeFilename(filename, "lst");
      if (!saveLst(final_name, ctx->io_ctx.lines, ctx->io_ctx.line_count)) {
        showMessage(GTK_MESSAGE_ERROR, ".lst 파일을 저장할 수 없습니다.");
      } else {
        showMessage(GTK_MESSAGE_INFO, ".lst 파일이 저장되었습니다.");
        g_free(ctx->file_ctx.lst_filename);
        ctx->file_ctx.lst_filename = g_strdup(final_name);
      }
      g_free(final_name);
      g_free(filename);
    }
  }

  gtk_widget_destroy(dialog);
}