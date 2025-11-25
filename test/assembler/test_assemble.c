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
      "           dup",        "           str 2 11",   "           str 2 0",
      "$$0        nop",        "           lod 2 0",    "           lod 1 0",
      "           lt",         "           fjp $$1",    "           lod 2 0",
      "           lda 2 1",    "           add",        "           lod 2 11",
      "           sti",        "           lod 2 0",    "           lda 2 1",
      "           add",        "           ldp",        "           lod 2 0",
      "           push",       "           lda 2 1",    "           push",
      "           call sub",   "           sti",        "           lod 2 11",
      "           lod 2 0",    "           lda 2 1",    "           add",
      "           ldi",        "           add",        "           str 2 11",
      "           lod 2 0",    "           ldc 1",      "           add",
      "           str 2 0",    "           ujp $$0",    "$$1        nop",
      "           ldp",        "           lod 2 11",   "           push",
      "           call write", "           ret",        "sub        proc 3",
      "           sym 3 0 1",  "           sym 3 1 1",  "           sym 3 2 1",
      "           ldp",        "           lda 3 2",    "           push",
      "           call read",  "           lod 3 2",    "           lod 3 0",
      "           lod 3 1",    "           add",        "           ldi",
      "           add",        "           str 3 2",    "           lod 3 2",
      "           ret"};

  int line_count = sizeof(lines) / sizeof(lines[0]);

  initVMContext();
  VMContext *ctx = getVMContext();

  if (assemble(lines, line_count, NULL) != ASSEMBLE_ERR_NONE) {
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

  printf("\n========= Func list =========\n");
  for (int i = 0; i < ctx->func_list.count; i++) {
    FuncInfo *func = &ctx->func_list.items[i];
    printf("Func: name=%s, param cnt=%d, is start=%d, func block=%d, "
           "start_addr=%d, end_addr=%d\n",
           func->name, func->param_cnt, func->is_start, func->func_block,
           func->start_addr, func->end_addr);
  }

  freeVMContext();
  return 0;
}

static int checkAssemble(int idx, char **lines, int line_count, int expected) {
  initVMContext();
  int res = assemble(lines, line_count, NULL);

  if (res != expected) {
    printf("[FAIL] assemble() fail test %d failed (expected %d, got %d)\n\n",
           idx, expected, res);
    freeVMContext();
    return 1;
  }

  printf("[PASS] assemble() fail test %d passed\n\n", idx);
  freeVMContext();
  return 0;
}

static int lineCount(char *lines[]) {
  int count = 0;
  while (lines[count] != NULL)
    count++;
  return count;
}

int testAssembleFailures() {
  TestCase tests[] = {
      {ASSEMBLE_ERR_INVALID_FORMAT, {"           badcmd 10", NULL}},
      {ASSEMBLE_ERR_VAR_DUP,
       {"main       proc 1", "           sym 1 0 1", "           sym 1 0 2",
        NULL}},
      {ASSEMBLE_ERR_VAR_UNDEF, {"           lod 1 0", NULL}},
      {ASSEMBLE_ERR_LABEL_DUP,
       {"main       proc 1", "main       proc 1", "           ret", NULL}},
      {ASSEMBLE_ERR_LABEL_UNDEF, {"           call notexist", NULL}},
      {ASSEMBLE_ERR_ARG_COUNT, {"           ldc", NULL}},
      {ASSEMBLE_ERR_ARG_TYPE, {"           ldc abc", NULL}},
      {ASSEMBLE_ERR_RETURN, {"main       proc 1", NULL}},
      {ASSEMBLE_ERR_PROC, {"           ret", NULL}},
  };

  int failures = 0;
  int test_count = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < test_count; i++) {
    failures += checkAssemble(i + 1, tests[i].lines, lineCount(tests[i].lines),
                              tests[i].expected);
  }
  return failures;
}