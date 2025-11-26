#include "core/instruction.h"
#include "core/opcode.h"
#include "core/vm_context.h"
#include "io_utils/io_utils.h"
#include "record/record.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int saveLst(const char *path, char **lines, int line_count) {
  int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0)
    return 0;

  int nbytes;
  char buf[LINE_BUFFER_LEN];
  VMContext *ctx = getVMContext();
  OutputBuffer *output = &ctx->output_buffer;
  int op_count;
  const OpInfo *op = getOpInfo(&op_count);

  const char *src_header = "==== 원본 코드 ====";
  nbytes = snprintf(buf, sizeof(buf), "%-6s %-29s %-10s %-6s %-8s\n", "",
                    src_header, "Encoded", "Opcode", "Operand");
  write(fd, buf, nbytes);

  // 역매핑
  int rev[line_count];
  memset(rev, 0xFF, sizeof(rev)); // -1로 초기화
  for (int i = 0; i < ctx->code_len; i++) {
    int src = ctx->source_map.line[i];
    rev[src] = i;
  }

  for (int i = 0; i < line_count; i++) {
    int asm_idx = rev[i];

    if (asm_idx == -1) {
      nbytes = snprintf(buf, sizeof(buf), "[%04d] %-24s\n", i + 1, lines[i]);
      write(fd, buf, nbytes);
      continue;
    }

    int inst = ctx->memory[asm_idx];
    int group = 0;
    int g_idx = 0;
    int operand = 0;
    decodeInst(inst, &group, &g_idx, &operand);

    nbytes = snprintf(buf, sizeof(buf), "[%04d] %-24s 0x%08X %-6d %-9d\n",
                      i + 1, lines[i], inst, group * 10 + g_idx, operand);

    write(fd, buf, nbytes);
  }

  // 실행 결과
  nbytes = snprintf(buf, sizeof(buf), "\n===========실행 결과=========\n");
  write(fd, buf, nbytes);
  write(fd, output->data, output->length);

  // 명령어 사용 횟수
  nbytes = snprintf(buf, sizeof(buf), "\n\n========명령어 사용 횟수======\n");
  write(fd, buf, nbytes);
  for (int i = 4; i < op_count; i++) {
    nbytes = snprintf(buf, sizeof(buf), "%-5s = %3d    ", op[i].name,
                      ctx->stat.inst_use_count[op[i].opcode]);
    write(fd, buf, nbytes);
    if (i % 3 == 0)
      write(fd, "\n", sizeof(char));
    else
      write(fd, "  ", sizeof(char));
  }

  // 명령어 실행 횟수
  nbytes = snprintf(buf, sizeof(buf), "\n======명령어 실행 횟수======\n");
  write(fd, buf, nbytes);
  for (int i = 4; i < op_count; i++) {
    nbytes = snprintf(buf, sizeof(buf), "%-5s = %3d    ", op[i].name,
                      ctx->stat.inst_run_count[op[i].opcode]);
    write(fd, buf, nbytes);
    if (i % 3 == 0)
      write(fd, "\n", sizeof(char));
    else
      write(fd, "  ", sizeof(char));
  }

  // 기타 통계
  nbytes = snprintf(buf, sizeof(buf), "\n======기타 통계======\n");
  write(fd, buf, nbytes);
  nbytes = snprintf(buf, sizeof(buf), "%-5s = %3d\n", "메모리 접근 횟수",
                    ctx->stat.memory_access_count);
  write(fd, buf, nbytes);

  close(fd);

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
