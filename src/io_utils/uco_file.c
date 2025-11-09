#include "io_utils/io_utils.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// .uco 파일에서 라인 단위로 읽어오기
int loadUco(const char *path, char ***lines, int *line_count) {
  int fd = open(path, O_RDONLY);
  if (fd < 0)
    return 0;

  int capacity = 100;
  *lines = malloc(sizeof(char *) * capacity);
  if (!*lines) {
    close(fd);
    return 0;
  }
  *line_count = 0;

  char buffer[256];
  ssize_t n;
  int pos = 0;
  char c;
  while ((n = read(fd, &c, 1)) > 0) {
    if (c == '\n') {
      buffer[pos] = '\0';
      if (*line_count >= capacity) {
        capacity *= 2;
        char **tmp = realloc(*lines, sizeof(char *) * capacity);
        if (!tmp) {
          freeUco(*lines, *line_count);
          close(fd);
          return 0;
        }
        *lines = tmp;
      }
      (*lines)[*line_count] = malloc(pos + 1);
      memcpy((*lines)[*line_count], buffer, pos + 1);
      (*line_count)++;
      pos = 0;
    } else if (pos < sizeof(buffer) - 1) {
      buffer[pos++] = c;
    }
  }

  if (pos > 0) {
    buffer[pos] = '\0';
    (*lines)[*line_count] = malloc(pos + 1);
    memcpy((*lines)[*line_count], buffer, pos + 1);
    (*line_count)++;
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