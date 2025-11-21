#include "core/inst.h"
#include "core/opcode.h"
#include "core/vm_context.h"
#include "io_utils/io_utils.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *inst_names[] = {
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
  nbytes = snprintf(buf, sizeof(buf), "%19s %8s\n",
                    "=======원본 코드=======", "==어셈블 결과==");
  write(fd, buf, nbytes);
  for (int i = 0; i < lines->line_count; i++) {
    nbytes = snprintf(buf, sizeof(buf), "%19s %8d\n", lines->ucode_lines[i],
                      lines->opcode[i]);
    write(fd, buf, nbytes);
  }

  // 실행 결과
  nbytes = snprintf(buf, sizeof(buf), "===========실행 결과=========\n");
  write(fd, buf, nbytes);
  write(fd, output->data, output->length);

  // 명령어 사용 횟수
  nbytes = snprintf(buf, sizeof(buf), "===========명령어 사용 횟수=========\n");
  write(fd, buf, strlen(buf));
  for (int i = 0; i < OPCODE_MAX; i++) {
    // TODO: 명령어 사용 횟수 넣기
  }

  // 명령어 실행 횟수
  nbytes = snprintf(buf, sizeof(buf), "======명령어 실행 횟수======\n");
  write(fd, buf, nbytes));
  for (int i = 0; i < OPCODE_MAX; i++) {
    nbytes = snprintf(buf, sizeof(buf), "%5s = ", inst_names[i]);
    write(fd, buf, nbytes);
    write(fd, &ctx->stat.inst_run_count[i], sizeof(int));
    if ((i + 1) % 3 == 0)
      write(fd, "\n", sizeof(char));
  }

  close(fd);
}
