#include "io_utils/io_utils.h"

#include "core/constants.h" // LINE_BUFFER_LEN
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int ensureCapacity(char ***lines, int *capacity, int line_count) {
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

int appendLine(char ***lines, int *line_count, int *capacity,
               const char *buffer, int len) {
  if (len <= 0)
    return 1;

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
  int fd = open(path, O_RDONLY);

  if (fd < 0)
    return 0;

  int capacity = INIT_LINE_CAPACITY;
  char **list = malloc(sizeof(char *) * capacity);
  if (!list) {
    close(fd);
    return 0;
  }

  *lines = list;
  *line_count = 0;

  char buffer[LINE_BUFFER_LEN];
  int pos = 0;

  ssize_t n;
  char c;

  while ((n = read(fd, &c, 1)) > 0) {
    if (c == '\n') {
      if (!appendLine(lines, line_count, &capacity, buffer, pos)) {
        freeUco(*lines, *line_count);
        close(fd);
        return 0;
      }
      pos = 0;
    } else if (c != '\r') { // CR 제거
      if (pos < (int)sizeof(buffer) - 1)
        buffer[pos++] = c;
    }
  }

  if (pos > 0) {
    if (!appendLine(lines, line_count, &capacity, buffer, pos)) {
      freeUco(*lines, *line_count);
      close(fd);
      return 0;
    }
  }

  close(fd);
  return 1;
}

// .uco 전체 덮어쓰기로 저장하기
int saveUco(const char *path, const char *content) {
  int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0)
    return 0;

  size_t len = strlen(content);
  ssize_t written = 0;
  while (written < len) {
    ssize_t n = write(fd, content + written, len - written);
    if (n <= 0) {
      close(fd);
      return 0; // 쓰기 실패
    }
    written += n;
  }

  close(fd);
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