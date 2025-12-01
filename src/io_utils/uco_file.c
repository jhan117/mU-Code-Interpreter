#include "io_utils/io_utils.h"

#include "core/constants.h" // INIT_LINE_CAPACITY, LINE_BUFFER_LEN
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int ensureCapacity(char ***lines, int *capacity, int line_count) {
  if (line_count < *capacity)
    return 1;

  int new_cap = (*capacity) * 2;
  char **tmp = realloc(*lines, sizeof(char *) * new_cap);
  if (!tmp)
    return 0;

  *lines = tmp;
  *capacity = new_cap;
  return 1;
}

static int appendLine(char ***lines, int *line_count, int *capacity,
                      const char *buffer, int len) {

  if (!ensureCapacity(lines, capacity, *line_count))
    return 0;

  char *dst = malloc(len + 1);
  if (!dst)
    return 0;

  memcpy(dst, buffer, len);
  dst[len] = '\0';

  (*lines)[*line_count] = dst;
  (*line_count)++;
  return 1;
}

// .uco 파일에서 라인 단위로 읽어오기
int loadUco(const char *path, char ***lines, int *line_count) {
  int fd = OpenFile(path, O_RDONLY);

  // 라인 동적 할당
  int capacity = INIT_LINE_CAPACITY;
  *lines = malloc(sizeof(char *) * capacity);
  if (!*lines) {
    CloseFile(fd);
    return 0;
  }

  *line_count = 0;
  char buffer[LINE_BUFFER_LEN];
  int pos = 0;

  ssize_t n;
  char c;

  while ((n = ReadFile(fd, &c, 1)) > 0) {
    if (c == '\n') {
      if (!appendLine(lines, line_count, &capacity, buffer, pos)) {
        freeUco(*lines, *line_count);
        CloseFile(fd);
        return 0;
      }
      pos = 0;
      buffer[0] = '\0';
    } else if (c != '\r') { // CR 제거
      if (pos < sizeof(buffer) - 1)
        buffer[pos++] = c;
    }
  }

  if (pos > 0) {
    if (!appendLine(lines, line_count, &capacity, buffer, pos)) {
      freeUco(*lines, *line_count);
      CloseFile(fd);
      return 0;
    }
  }

  CloseFile(fd);
  return 1;
}

char *joinLines(char **lines, int line_count) {
  if (line_count == 0 || !lines)
    return NULL;

  // 전체 길이 계산
  int total_len = 0;
  for (int i = 0; i < line_count; i++) {
    if (lines[i]) {
      total_len += strlen(lines[i]) + 1; // 문자열 길이 + '\n'
    }
  }

  // 마지막 '\0' 포함
  char *result = malloc(total_len + 1);
  if (!result)
    return NULL;

  char *p = result;
  for (int i = 0; i < line_count; i++) {
    if (lines[i]) {
      int len = strlen(lines[i]);
      strcopy(p, lines[i], len + 1);
      p += len;
      if (i != line_count - 1)
        *p++ = '\n';
    }
  }
  *p = '\0';
  return result; // free 필요
}

// .uco 전체 덮어쓰기로 저장하기
int saveUco(const char *path, char **lines, int line_count) {
  int fd = OpenFile(path, O_WRONLY | O_CREAT | O_TRUNC);

  char *content = joinLines(lines, line_count);
  if (!content) {
    CloseFile(fd);
    return 0;
  }

  for (int i = 0; i < line_count; i++) {
    free(lines[i]);
  }
  free(lines);

  int len = strlen(content);
  ssize_t written = 0;
  while (written < len) {
    ssize_t n = WriteFile(fd, content + written, len - written);
    written += n;
  }

  free(content);
  CloseFile(fd);
  return 1;
}

// 메모리 해제
void freeUco(char **lines, int line_count) {
  if (!lines)
    return;

  for (int i = 0; i < line_count; i++) {
    free(lines[i]);
  }
  free(lines);
}