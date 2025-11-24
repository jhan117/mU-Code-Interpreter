#include "test.h"

#include "assembler/assemble.h"
#include <stdio.h>
#include <stdlib.h>

int testAssembleSuccess() {
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
      "           ret 0",      "sub        proc 3",     "           sym 3 0 1",
      "           sym 3 1 1",  "           sym 3 2 1",  "           ldp",
      "           lda 3 2",    "           push",       "           call read",
      "           lod 3 2",    "           lod 3 0",    "           lod 3 1",
      "           add",        "           ldi",        "           add",
      "           str 3 2",    "           lod 3 2",    "           ret 2"};

  int line_count = sizeof(lines) / sizeof(lines[0]);

  initVMContext();
  VMContext *ctx = getVMContext();

  if (assemble(lines, line_count) != ASSEMBLE_ERR_NONE) {
    printf("[FAIL] assemble() test failed\n");
    return 1;
  }

  printf("[PASS] assemble() test passed\n");

  printf("\n========= Assembled memory =========\n");
  for (int i = 0; i < ctx->code_len; i++) {
    int inst = ctx->memory[i];
    int op_group = (inst >> 29) & 0x7;
    int op_group_idx = (inst >> 26) & 0x7;
    int operand = inst & 0x03FFFFFF;

    decodeInst(inst, &op_group, &op_group_idx, &operand);

    printf("%04d: opcode=%d%d operand=%d\n", i, op_group, op_group_idx,
           operand);
  }

  // code_len 확인
  if (ctx->memory[ctx->code_len])
    printf("Code Not End\n");

  printf("\n========= Label list =========\n");
  for (int i = 0; i < ctx->label_list.count; i++) {
    Label *label = &ctx->label_list.labels[i];
    printf("Label: %s, addr=%d\n", label->name, label->addr);
  }

  printf("\n========= Symbol list =========\n");
  printf("Global Symbol Count: %d\n", ctx->g_var_cnt);
  for (int i = 0; i < ctx->symbol_list.count; i++) {
    Symbol *sym = &ctx->symbol_list.symbols[i];
    printf("Symbol[%d]: block=%d, offset=%d, size=%d\n", sym->index, sym->block,
           sym->offset, sym->size);
  }

  freeVMContext();
  return 0;
}

static int checkAssemble(int idx, char **lines, int line_count, int expected) {
  initVMContext();
  int res = assemble(lines, line_count);
  if (res != expected) {
    printf("[FAIL] assemble() fail test %d failed\n\n", idx);
    freeVMContext();
    return 1;
  } else {
    printf("[PASS] assemble() fail test %d passed\n\n", idx);
    freeVMContext();
    return 0;
  }
}

int testAssembleFailures() {
  int failures = 0;

  char *lines1[] = {"           badcmd 10"}; // ASSEMBLE_ERR_INVALID_FORMAT
  char *lines2[] = {
      "           sym 1 0 1",
      "           sym 1 0 2",
  }; // ASSEMBLE_ERR_VAR_DUP
  char *lines3[] = {"           lod 1 0"}; // ASSEMBLE_ERR_VAR_UNDEF
  char *lines4[] = {
      "main       proc 1",
      "main       proc 1",
  }; // ASSEMBLE_ERR_LABEL_DUP
  char *lines5[] = {"           call notexist"}; // ASSEMBLE_ERR_LABEL_UNDEF
  char *lines6[] = {"           ldc"};           // ASSEMBLE_ERR_ARG_COUNT
  char *lines7[] = {"           ldc abc"};       // ASSEMBLE_ERR_ARG_TYPE

  char *lines8[INIT_MEMORY_SIZE + 1]; // ASSEMBLE_ERR_MEMORY
  for (int j = 0; j < INIT_MEMORY_SIZE + 1; j++)
    lines8[j] = "           ldc 0";

  TestCase tests[] = {
      {1, lines1, 1, ASSEMBLE_ERR_INVALID_FORMAT},
      {2, lines2, 2, ASSEMBLE_ERR_VAR_DUP},
      {3, lines3, 1, ASSEMBLE_ERR_VAR_UNDEF},
      {4, lines4, 2, ASSEMBLE_ERR_LABEL_DUP},
      {5, lines5, 1, ASSEMBLE_ERR_LABEL_UNDEF},
      {6, lines6, 1, ASSEMBLE_ERR_ARG_COUNT},
      {7, lines7, 1, ASSEMBLE_ERR_ARG_TYPE},
      {8, lines8, INIT_MEMORY_SIZE + 1, ASSEMBLE_ERR_MEMORY},
  };

  for (int i = 0; i < 8; i++)
    failures += checkAssemble(tests[i].id, tests[i].lines, tests[i].count,
                              tests[i].expected);

  return failures;
}