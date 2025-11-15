#include "test.h"

#include "assemble.h"
#include <stdio.h>
#include <stdlib.h>

const char *assemble_error_str(int code) {
  switch (code) {
  case ASSEMBLE_OK:
    return "OK";
  case ASSEMBLE_ERR_INVALID_FORMAT:
    return "INVALID_FORMAT";
  case ASSEMBLE_ERR_VAR_DUP:
    return "VAR_DUP";
  case ASSEMBLE_ERR_LABEL_DUP:
    return "LABEL_DUP";
  case ASSEMBLE_ERR_LABEL_MISSING:
    return "LABEL_MISSING";
  case ASSEMBLE_ERR_ARG_COUNT:
    return "ARG_COUNT";
  case ASSEMBLE_ERR_ARG_TYPE:
    return "ARG_TYPE";
  case ASSEMBLE_ERR_VAR_UNDEF:
    return "VAR_UNDEF";
  case ASSEMBLE_ERR_MEMORY:
    return "MEMORY_ERROR";
  default:
    return "UNKNOWN_ERROR";
  }
}

int test_assemble_success() {
  char *lines[] = {
      "           bgn 1",      "           sym  1 0 1", "           ldc  10",
      "           str  1 0",   "           ldp",        "           call main",
      "           end",        "main       proc 12",    "           sym 2 0 1",
      "           sym 2 1 10", "           sym 2 11 1", "           ldc 0",
      "           str 2 11",   "           str 2 0",    "$$0        nop",
      "           lod 2 0",    "           lod 1 0",    "           lt",
      "           fjp $$1",    "           lod 2 0",    "           lda 2 1",
      "           add",        "           lod 2 11",   "           sti",
      "           lod 2 0",    "           lda 2 1",    "           add",
      "           ldp",        "           lod 2 0",    "           push",
      "           lda 2 1",    "           push",       "           call sub",
      "           sti",        "           lod 2 11",   "           lod 2 0",
      "           lda 2 1",    "           add",        "           ldi",
      "           add",        "           str 2 11",   "           lod 2 0",
      "           ldc 1",      "           add",        "           str 2 0",
      "           ujp $$0",    "$$1        nop",        "           ldp",
      "           lod 2 11",   "           push",       "           call write",
      "           ret",        "sub        proc 3",     "           sym 3 0 1",
      "           sym 3 1 1",  "           sym 3 2 1",  "           ldp",
      "           lda 3 2",    "           push",       "           call read",
      "           lod 3 2",    "           lod 3 0",    "           lod 3 1",
      "           add",        "           ldi",        "           add",
      "           str 3 2",    "           lod 3 2",    "           ret"};

  int line_count = sizeof(lines) / sizeof(lines[0]);

  initVMContext();

  int res = assemble(lines, line_count);

  if (res != ASSEMBLE_OK) {
    printf("[FAIL] assemble() returned %d (%s)\n", res,
           assemble_error_str(res));
    return 1;
  }

  printf("[PASS] assemble() test passed\n");

  VMContext *ctx = getVMContext();

  printf("\n========= Assembled memory =========\n");
  for (int i = 0; i < INIT_MEMORY_SIZE; i++) {
    int inst = ctx->memory[i];
    int opcode = (inst >> 26) & 0x3F;
    int operand = inst & 0x03FFFFFF;

    // 26비트 signed 처리 ==> 음수 변환
    if (operand & (1 << 25))
      operand |= ~0x03FFFFFF;

    if (opcode == 0 && operand == 0)
      break;

    // 2주소 명령어 출력 ==> 뭔가 좀 이상하긴 한데 ㅋ
    int block = (operand >> 13) & 0x1FFF;
    int offset = operand & 0x1FFF;

    if (offset != 0 || block != 0) {
      printf("%04d: opcode=%02d operand=%d (block=%d, offset=%d)\n", i, opcode,
             operand, block, offset);
    } else {
      printf("%04d: opcode=%02d operand=%d\n", i, opcode, operand);
    }
  }

  printf("\n========= Label list =========\n");
  for (int i = 0; i < ctx->labels.count; i++) {
    Label *lbl = &ctx->labels.labels[i];
    printf("Label: %s, addr=%d\n", lbl->name, lbl->addr);
  }

  printf("\n========= Symbol list =========\n");
  for (int i = 0; i < ctx->symbols.count; i++) {
    Symbol *sym = &ctx->symbols.symbols[i];
    printf("Symbol: block=%d, offset=%d, size=%d\n", sym->block, sym->offset,
           sym->size);
  }

  freeVMContext();

  return 0;
}

static int checkAssemble(char **lines, int line_count, int expected,
                         const char *desc) {
  initVMContext();
  int res = assemble(lines, line_count);
  if (res != expected) {
    printf("[FAIL] %s: expected %s, got %s\n\n", desc,
           assemble_error_str(expected), assemble_error_str(res));
    freeVMContext();
    return 1;
  } else {
    printf("[PASS] %s\n\n", desc);
    freeVMContext();
    return 0;
  }
}

int test_assemble_failures() {
  int failures = 0;
  char *lines1[] = {"           badcmd 10"};
  failures +=
      checkAssemble(lines1, 1, ASSEMBLE_ERR_INVALID_FORMAT, "Invalid command");

  char *lines2[] = {"           ldc abc"};
  failures +=
      checkAssemble(lines2, 1, ASSEMBLE_ERR_ARG_TYPE, "Invalid number operand");

  char *lines3[] = {"main       proc 1", "main       proc 1"};
  failures +=
      checkAssemble(lines3, 2, ASSEMBLE_ERR_LABEL_DUP, "Duplicate label");

  char *lines4[] = {"           ldc"};
  failures += checkAssemble(lines4, 1, ASSEMBLE_ERR_ARG_COUNT,
                            "Missing operand for ldc");

  char *lines5[] = {"           call 123"};
  failures += checkAssemble(lines5, 1, ASSEMBLE_ERR_ARG_TYPE,
                            "Invalid operand type for call");

  char *lines6[] = {"           bgn 1", "           sym 1 0 1",
                    "           sym 1 0 2"};
  failures +=
      checkAssemble(lines6, 3, ASSEMBLE_ERR_VAR_DUP, "Duplicate symbol");

  char *lines7[INIT_MEMORY_SIZE + 1];
  for (int j = 0; j < INIT_MEMORY_SIZE + 1; j++)
    lines7[j] = "           ldc 0";
  failures += checkAssemble(lines7, INIT_MEMORY_SIZE + 1, ASSEMBLE_ERR_MEMORY,
                            "Memory overflow");

  char *lines8[] = {"           ujp notexist"};
  failures += checkAssemble(lines8, 1, ASSEMBLE_ERR_LABEL_MISSING,
                            "Undefined label reference");

  return failures;
}