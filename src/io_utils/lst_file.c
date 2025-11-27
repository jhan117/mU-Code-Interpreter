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

int makeLst(const char **src_lines, int src_count, char ***out_lines,
            int *out_count) {
  VMContext *ctx = getVMContext();
  OutputBuffer *output = &ctx->output_buffer;
  int op_count;
  const OpInfo *op = getOpInfo(&op_count);

  // 결과 라인 배열 동적 할당
  int capacity = INIT_LINE_CAPACITY;
  *out_lines = malloc(sizeof(char *) * capacity);
  if (!*out_lines)
    return 0;
  *out_count = 0;

  char buf[LINE_BUFFER_LEN];

  // 헤더
  snprintf(buf, sizeof(buf), "%-6s %-29s %-10s %-6s %-8s\n", "",
           "==== 원본 코드 ====", "Encoded", "Opcode", "Operand");
  (*out_lines)[(*out_count)++] = strdup(buf);

  // 역매핑
  int *rev = malloc(sizeof(int) * src_count);
  if (!rev) {
    free(*out_lines);
    return 0;
  }
  for (int i = 0; i < src_count; i++)
    rev[i] = -1;
  for (int i = 0; i < ctx->code_len; i++) {
    int src = ctx->source_map.line[i];
    if (src >= 0 && src < src_count)
      rev[src] = i;
  }

  // 원본 코드 + 디코딩 결과
  for (int i = 0; i < src_count; i++) {
    int asm_idx = rev[i];
    if (asm_idx == -1) {
      snprintf(buf, sizeof(buf), "[%04d] %-24s\n", i + 1, src_lines[i]);
    } else {
      int inst = ctx->memory[asm_idx];
      int group = 0, g_idx = 0, operand = 0;
      decodeInst(inst, &group, &g_idx, &operand);
      snprintf(buf, sizeof(buf), "[%04d] %-24s 0x%08X %-6d %-9d\n", i + 1,
               src_lines[i], inst, group * 10 + g_idx, operand);
    }
    (*out_lines)[(*out_count)++] = strdup(buf);
  }

  // 실행 결과
  snprintf(buf, sizeof(buf), "\n===========실행 결과=========\n");
  (*out_lines)[(*out_count)++] = strdup(buf);
  if (output->data)
    (*out_lines)[(*out_count)++] = strdup(output->data);

  // 명령어 사용 횟수
  snprintf(buf, sizeof(buf), "\n\n========명령어 사용 횟수======\n");
  (*out_lines)[(*out_count)++] = strdup(buf);
  for (int i = 4; i < op_count; i++) {
    snprintf(buf, sizeof(buf), "%-5s = %3d    ", op[i].name,
             ctx->stat.inst_use_count[op[i].opcode]);
    (*out_lines)[(*out_count)++] = strdup(buf);
    if (i % 3 == 0)
      (*out_lines)[(*out_count)++] = strdup("\n");
    else
      (*out_lines)[(*out_count)++] = strdup("  ");
  }

  // 명령어 실행 횟수
  snprintf(buf, sizeof(buf), "\n======명령어 실행 횟수======\n");
  (*out_lines)[(*out_count)++] = strdup(buf);
  for (int i = 4; i < op_count; i++) {
    snprintf(buf, sizeof(buf), "%-5s = %3d    ", op[i].name,
             ctx->stat.inst_run_count[op[i].opcode]);
    (*out_lines)[(*out_count)++] = strdup(buf);
    if (i % 3 == 0)
      (*out_lines)[(*out_count)++] = strdup("\n");
    else
      (*out_lines)[(*out_count)++] = strdup("  ");
  }

  // 기타 통계
  snprintf(buf, sizeof(buf), "\n======기타 통계======\n");
  (*out_lines)[(*out_count)++] = strdup(buf);
  snprintf(buf, sizeof(buf), "%-5s = %3d\n", "메모리 접근 횟수",
           ctx->stat.memory_access_count);
  (*out_lines)[(*out_count)++] = strdup(buf);

  free(rev);
  return 1;
}

void freeLst(char **lines, int count) {
  if (!lines)
    return;
  for (int i = 0; i < count; i++)
    free(lines[i]);
  free(lines);
}

int saveLst(const char *path, char **lines, int line_count) {
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

// int loadLst(const char *path, char ***lines, int *line_count) {
//   int fd = open(path, O_RDONLY);
//   if (fd < 0)
//     return 0;

//   int capacity = 100;
//   *lines = malloc(sizeof(char *) * capacity);
//   if (!*lines) {
//     close(fd);
//     return 0;
//   }
//   *line_count = 0;

//   char buffer[256];
//   ssize_t n;
//   int pos = 0;
//   char c;
//   while ((n = read(fd, &c, 1)) > 0) {
//     if (c == '\n') {
//       buffer[pos] = '\0';
//       if (*line_count >= capacity) {
//         capacity *= 2;
//         char **tmp = realloc(*lines, sizeof(char *) * capacity);
//         if (!tmp) {
//           freeLst(*lines, *line_count);
//           close(fd);
//           return 0;
//         }
//         *lines = tmp;
//       }
//       (*lines)[*line_count] = malloc(pos + 1);
//       memcpy((*lines)[*line_count], buffer, pos + 1);
//       (*line_count)++;
//       pos = 0;
//     } else if (pos < sizeof(buffer) - 1) {
//       buffer[pos++] = c;
//     }
//   }

//   if (pos > 0) {
//     buffer[pos] = '\0';
//     if (*line_count >= capacity) {
//       capacity *= 2;
//       char **tmp = realloc(*lines, sizeof(char *) * capacity);
//       if (!tmp) {
//         freeLst(*lines, *line_count);
//         close(fd);
//         return 0;
//       }
//       *lines = tmp;
//     }
//     (*lines)[*line_count] = malloc(pos + 1);
//     memcpy((*lines)[*line_count], buffer, pos + 1);
//     (*line_count)++;
//   }

//   close(fd);
//   return 1;
// }
