#include "core/inst.h"
#include "core/opcode.h"
#include "core/vm_context.h"
#include "io_utils/io_utils.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const int opcode_list[] = {
    OP_PROC, OP_RET, OP_LDP, OP_PUSH, OP_CALL, OP_UJP, OP_TJP, OP_FJP,
    OP_LOD,  OP_LDA, OP_LDC, OP_STR,  OP_LDI,  OP_STI, OP_GT,  OP_LT,
    OP_GE,   OP_LE,  OP_EQ,  OP_NE,   OP_AND,  OP_OR,  OP_ADD, OP_SUB,
    OP_MULT, OP_DIV, OP_MOD, OP_NOT,  OP_NEG};

static const int opcode_list_count =
    sizeof(opcode_list) / sizeof(opcode_list[0]);

static const char *opcode_names[] = {
    [OP_PROC] = "proc", [OP_RET] = "ret", [OP_LDP] = "ldp", [OP_PUSH] = "push",
    [OP_CALL] = "call", [OP_UJP] = "ujp", [OP_TJP] = "tjp", [OP_FJP] = "fjp",
    [OP_LOD] = "lod",   [OP_LDA] = "lda", [OP_LDC] = "ldc", [OP_STR] = "str",
    [OP_LDI] = "ldi",   [OP_STI] = "sti", [OP_GT] = "gt",   [OP_LT] = "lt",
    [OP_GE] = "ge",     [OP_LE] = "le",   [OP_EQ] = "eq",   [OP_NE] = "ne",
    [OP_AND] = "and",   [OP_OR] = "or",   [OP_ADD] = "add", [OP_SUB] = "sub",
    [OP_MULT] = "mult", [OP_DIV] = "div", [OP_MOD] = "mod", [OP_NOT] = "not",
    [OP_NEG] = "neg"};

int saveLst(const char *path, UCodeLines *lines) {
  int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0)
    return 0;

  int nbytes;
  char buf[100];
  VMContext *ctx = getVMContext();
  OutputBuffer *output = getOutputBuffer();

  // 어셈블결과, 인코딩 결과
  const char *src_header = "======= 원본 코드 =======";
  const char *opcode_header = "==== 어셈블 결과 ====";
  nbytes = snprintf(buf, sizeof(buf), "%-35s %s\n", src_header, opcode_header);
  write(fd, buf, nbytes);
  for (int i = 0; i < lines->line_count; i++) {
    nbytes = snprintf(buf, sizeof(buf), "%-35s %12d\n", lines->ucode_lines[i],
                      lines->opcode[i]);
    write(fd, buf, nbytes);
  }

  // 실행 결과
  nbytes = snprintf(buf, sizeof(buf), "===========실행 결과=========\n");
  write(fd, buf, nbytes);
  write(fd, output->data, output->length);

  // 명령어 사용 횟수
  nbytes = snprintf(buf, sizeof(buf), "\n========명령어 사용 횟수======\n");
  write(fd, buf, nbytes);
  for (int i = 0; i < opcode_list_count; i++) {
    // TODO: 명령어 사용 횟수 넣기
  }

  // 명령어 실행 횟수
  nbytes = snprintf(buf, sizeof(buf), "\n======명령어 실행 횟수======\n");
  write(fd, buf, nbytes);
  for (int i = 0; i < opcode_list_count; i++) {
    nbytes = snprintf(buf, sizeof(buf), "%-5s = %3d    ",
                      opcode_names[opcode_list[i]],
                      ctx->stat.inst_run_count[opcode_list[i]]);
    write(fd, buf, nbytes);
    if ((i + 1) % 3 == 0)
      write(fd, "\n", sizeof(char));
    else
      write(fd, "  ", sizeof(char));
  }

  close(fd);
  freeOutBuffer();

  return 1;
}

int loadLst(const char *path, char ***lines, int *line_count) {
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
          freeLst(*lines, *line_count);
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
    if (*line_count >= capacity) {
      capacity *= 2;
      char **tmp = realloc(*lines, sizeof(char *) * capacity);
      if (!tmp) {
        freeLst(*lines, *line_count);
        close(fd);
        return 0;
      }
      *lines = tmp;
    }
    (*lines)[*line_count] = malloc(pos + 1);
    memcpy((*lines)[*line_count], buffer, pos + 1);
    (*line_count)++;
  }

  close(fd);
  return 1;
}

void freeLst(char **lines, int line_count) {
  if (!lines)
    return;

  for (int i = 0; i < line_count; i++) {
    free(lines[i]);
  }
  free(lines);
}
