#include "runner.h"
#include "core/inst.h"
#include "core/opcode.h"
#include "core/vm_context.h"

#include <stdio.h>

static const int opcode_base[TOTAL_OPCODE_GROUPS] = {OP_PROC, OP_UJP, OP_LOD,
                                                     OP_GT, OP_ADD};

static const int mem_access[OPCODE_MAX] = {
    [OP_RET] = 2, [OP_PUSH] = 1, [OP_CALL] = 2, [OP_LOD] = 1,
    [OP_LDA] = 1, [OP_STR] = 1,  [OP_LDI] = 1,  [OP_STI] = 1};

void step() {
  VMContext *ctx = getVMContext();
  int inst = ctx->memory[ctx->pc++];
  int group_code = decodeGroup(inst);
  int opcode = opcode_base[group_code] + decodeOpcode(inst);
  ctx->stat.inst_run_count[opcode]++;
  ctx->stat.memory_access_count += mem_access[opcode];
  ctx->inst_group[group_code].execInst(inst);
  return;
}

void printError() {
  VMContext *ctx = getVMContext();
  if (ctx->bp == -1)
    printf("[INFO] runner stopped (bp == -1)\n");

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
}

void readyToRun() {
  VMContext *ctx = getVMContext();
  ctx->cs = 0;
  ctx->ds = 1000; // 나중에 수정
  ctx->ss = 2000; // 나중에 수정
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
  updateSymbols();

  while (1) {
    if (ctx->flags != 0 || ctx->bp == -1) {
      printError();
      return -1;
    }
    step();
    saveChanges();
  }
}
