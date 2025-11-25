#include "runner.h"
#include "assemble.h"
#include "core/inst.h"
#include "core/instruction.h"
#include "core/opcode.h"
#include "core/vm_context.h"

#include <stdio.h>
#include <stdlib.h>

static const int mem_access[OPCODE_MAX] = {
    [OP_RET] = 2, [OP_PUSH] = 1, [OP_CALL] = 2, [OP_LOD] = 1,
    [OP_LDA] = 1, [OP_STR] = 1,  [OP_LDI] = 1,  [OP_STI] = 1};

void step() {
  VMContext *ctx = getVMContext();
  int inst = ctx->memory[ctx->pc++];
  int group_code;
  int g_idx;
  int operand;
  decodeInst(inst, &group_code, &g_idx, &operand);
  int opcode = group_code * 10 + g_idx;
  ctx->stat.inst_run_count[opcode]++;
  ctx->stat.memory_access_count += mem_access[opcode];
  ctx->inst_group[group_code].execInst(inst);
  return;
}

void printError(int prev_pc) {
  VMContext *ctx = getVMContext();

  if (ctx->flags == 0)
    return;

  if (ctx->flags & ERR_INVALID_ADDR)
    printf("[ERROR] Invalid address access\n");
  if (ctx->flags & ERR_INVALID_PC)
    printf("[ERROR] Invalid program counter\n");
  if (ctx->flags & ERR_INVALID_BP)
    printf("[ERROR] Invalid base pointer\n");
  if (ctx->flags & ERR_STACK_OVERFLOW)
    printf("[ERROR] Stack overflow\n");
  if (ctx->flags & ERR_STACK_UNDERFLOW)
    printf("[ERROR] Stack underflow\n");
  if (ctx->flags & ERR_CPU_STACK_OVERFLOW)
    printf("[ERROR] CPU stack overflow\n");
  if (ctx->flags & ERR_CPU_STACK_UNDERFLOW)
    printf("[ERROR] CPU stack underflow\n");

  if (prev_pc >= 0 && prev_pc < ctx->code_len) {
    char inst[32];
    int inst_val = ctx->memory[prev_pc];
    int group;
    int g_idx;
    int operand;
    decodeInst(inst_val, &group, &g_idx, &operand);
    const char *opcode_name = findOpInfoByOpcode(group * 10 + g_idx)->name;
    snprintf(inst, sizeof(inst), "%s %d", opcode_name, operand);
    printf("[DEBUG] instruction : %s | pc : %d | bp : %d | sp : %d\n", inst,
           ctx->pc, ctx->bp, ctx->sp);
  }
}

void readyToRun() {
  VMContext *ctx = getVMContext();
  ctx->cs = 0;
  ctx->ds = ctx->code_len;
  ctx->ss = ctx->code_len + ctx->g_var_cnt;
  ctx->pc = 0;

  int stack_top = INIT_MEMORY_SIZE - 1;
  ctx->bp = stack_top;
  ctx->sp = stack_top - 2;
  ctx->memory[stack_top - 1] = 0;
  ctx->memory[stack_top] = -1;
}

int runner() {
  VMContext *ctx = getVMContext();
  initOutBuffer();
  initSnapshot();
  initSnapshotList();
  readyToRun();

  int prev_pc = 0;

  while (1) {
    if (ctx->bp == -1) {
      printf("[INFO] runner stopped\n");
      return 0;
    }
    if (ctx->flags != 0) {
      printError(prev_pc);
      return -1;
    }
    prev_pc = ctx->pc;
    step();
    saveChanges();
  }
}
