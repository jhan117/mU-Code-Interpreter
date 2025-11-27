#include "core/instruction.h"
#include "core/opcode.h"
#include "core/vm_context.h"
#include "io_utils/io_utils.h"
#include "record/record.h"

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core/vm_context.h"
#include "io_utils/io_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int appendText(char **buf, int *len, int *cap, const char *text) {
  int n = strlen(text);
  if (*len + n + 1 > *cap) {
    int newcap = (*cap) * 2;
    while (newcap < *len + n + 1)
      newcap *= 2;
    char *newbuf = realloc(*buf, newcap);
    if (!newbuf)
      return 0;
    *buf = newbuf;
    *cap = newcap;
  }
  memcpy(*buf + *len, text, n);
  *len += n;
  (*buf)[*len] = '\0';
  return 1;
}

static int appendFormat(char **buf, int *len, int *cap, const char *fmt, ...) {
  char tmp[2048];
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
  va_end(ap);

  if (n < 0)
    return 0;
  if (n >= (int)sizeof(tmp))
    n = sizeof(tmp) - 1;

  tmp[n] = '\0';
  return appendText(buf, len, cap, tmp);
}

char *makeLstString(const char **src_lines, int src_count) {
  VMContext *ctx = getVMContext();
  OutputBuffer *output = &ctx->output_buffer;

  int op_count;
  const OpInfo *op = getOpInfo(&op_count);

  // 문자열 빌더 초기화
  int cap = 4096;
  int len = 0;
  char *buf = malloc(cap);
  if (!buf)
    return NULL;
  buf[0] = '\0';

  // 헤더
  if (!appendFormat(&buf, &len, &cap, "%-6s %-29s %-10s %-6s %-8s\n", "",
                    "==== 원본 코드 ====", "Encoded", "Opcode", "Operand"))
    return buf;

  // 역매핑 배열
  int *rev = malloc(sizeof(int) * src_count);
  if (!rev) {
    free(buf);
    return NULL;
  }

  for (int i = 0; i < src_count; i++)
    rev[i] = -1;

  for (int i = 0; i < ctx->code_len; i++) {
    int src = ctx->source_map.line[i];
    if (src >= 0 && src < src_count)
      rev[src] = i;
  }

  // 원본 코드 + decode 결과
  for (int i = 0; i < src_count; i++) {
    int asm_idx = rev[i];
    if (asm_idx < 0) {
      appendFormat(&buf, &len, &cap, "[%04d] %-24s\n", i + 1, src_lines[i]);
    } else {
      int inst = ctx->memory[asm_idx];
      int g = 0, gi = 0, operand = 0;
      decodeInst(inst, &g, &gi, &operand);

      appendFormat(&buf, &len, &cap, "[%04d] %-24s 0x%08X %-6d %-9d\n", i + 1,
                   src_lines[i], inst, g * 10 + gi, operand);
    }
  }

  // 실행 결과
  appendText(&buf, &len, &cap, "\n===========실행 결과=========\n");
  if (output->data)
    appendText(&buf, &len, &cap, output->data);

  // 명령어 사용 횟수
  appendText(&buf, &len, &cap, "\n\n========명령어 사용 횟수======\n");

  for (int i = 4; i < op_count; i++) {
    appendFormat(&buf, &len, &cap, "%-5s = %3d    ", op[i].name,
                 ctx->stat.inst_use_count[op[i].opcode]);

    if (i % 3 == 0)
      appendText(&buf, &len, &cap, "\n");
    else
      appendText(&buf, &len, &cap, "  ");
  }

  // 명령어 실행 횟수
  appendText(&buf, &len, &cap, "\n======명령어 실행 횟수======\n");

  for (int i = 4; i < op_count; i++) {
    appendFormat(&buf, &len, &cap, "%-5s = %3d    ", op[i].name,
                 ctx->stat.inst_run_count[op[i].opcode]);

    if (i % 3 == 0)
      appendText(&buf, &len, &cap, "\n");
    else
      appendText(&buf, &len, &cap, "  ");
  }

  // 기타 통계
  appendText(&buf, &len, &cap, "\n======기타 통계======\n");

  appendFormat(&buf, &len, &cap, "%-5s = %3d\n", "메모리 접근 횟수",
               ctx->stat.memory_access_count);

  free(rev);
  return buf;
}

int saveLst(const char *path, const char **src_lines, int src_count) {
  int fd = OpenFile(path, O_WRONLY | O_CREAT | O_TRUNC);
  if (fd < 0)
    return 0;

  char *content = makeLstString(src_lines, src_count);
  if (!content) {
    CloseFile(fd);
    return 0;
  }

  int len = strlen(content);
  int written = 0;

  while (written < len) {
    ssize_t n = WriteFile(fd, content + written, len - written);
    if (n <= 0) {
      free(content);
      CloseFile(fd);
      return 0;
    }
    written += n;
  }

  free(content);
  CloseFile(fd);
  return 1;
}