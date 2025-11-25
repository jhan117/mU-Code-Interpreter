#include "core/inst.h"
#include "core/stack_operations.h"
#include "core/vm_context.h"
#include "runner.h"
#include <stdio.h>

void proc(int arg) {
  VMContext *ctx = getVMContext();
  ctx->sp -= arg;
  if (checkError(ctx, NULL, NULL, NULL, &ctx->sp))
    return;
  return;
}

void ret(int arg) {
  VMContext *ctx = getVMContext();
  (void)arg;
  ctx->sp = ctx->bp;
  ctx->pc = ctx->memory[ctx->bp - 1];
  ctx->bp = ctx->memory[ctx->bp];
  if (checkError(ctx, NULL, &ctx->pc, &ctx->bp, NULL))
    return;
  return;
}

// 얘 할일 사라짐
void ldp(int arg) { (void)arg; }

void push(int arg) {
  VMContext *ctx = getVMContext();
  (void)arg;
  int item = popCPUStack();
  ctx->memory[ctx->sp--] = item;
  if (checkError(ctx, NULL, NULL, NULL, &ctx->sp))
    return;
  return;
}

void call(int arg) {
  VMContext *ctx = getVMContext();
  if (arg >= 0) {
    ctx->memory[ctx->sp] = ctx->bp;
    ctx->memory[ctx->sp - 1] = ctx->pc;
    ctx->bp = ctx->sp;
    ctx->sp = ctx->sp - 2;
    if (checkError(ctx, NULL, &arg, &ctx->bp, &ctx->sp))
      return;
    ctx->pc = arg;

    return;
  } else if (arg == -1) {
    Read();
  } else if (arg == -2) {
    Write();
  } else if (arg == -3) {
    lf();
  }
  return;
}

void ujp(int arg) {
  VMContext *ctx = getVMContext();
  ctx->pc = arg;
  if (checkError(ctx, NULL, &ctx->pc, NULL, NULL))
    return;
  return;
}

void tjp(int arg) {
  VMContext *ctx = getVMContext();
  if (popCPUStack()) {
    ctx->pc = arg;
    if (checkError(ctx, NULL, &ctx->pc, NULL, NULL))
      return;
  }
  return;
}

void fjp(int arg) {
  VMContext *ctx = getVMContext();
  if (!popCPUStack()) {
    ctx->pc = arg;
    if (checkError(ctx, NULL, &ctx->pc, NULL, NULL))
      return;
  }
  return;
}

// 아래 데이터 이동 연산자에서만 사용
static inline int resolveAddress(VMContext *ctx, int index) {
  if (ctx->symbol_list.symbols[index].block == GLOBAL_BLOCK)
    return ctx->ds + ctx->symbol_list.symbols[index].offset;
  else
    return ctx->bp - ctx->symbol_list.symbols[index].offset;
}

void lod(int arg) {
  VMContext *ctx = getVMContext();
  int addr = resolveAddress(ctx, arg);
  if (checkError(ctx, &addr, NULL, NULL, NULL))
    return;
  pushCPUStack(ctx->memory[addr]);
  return;
}

void lda(int arg) {
  VMContext *ctx = getVMContext();
  int addr = resolveAddress(ctx, arg);
  pushCPUStack(addr);
  return;
}

void str(int arg) {
  VMContext *ctx = getVMContext();
  int addr = resolveAddress(ctx, arg);
  if (checkError(ctx, &addr, NULL, NULL, NULL))
    return;
  int item = popCPUStack();
  ctx->memory[addr] = item;
  return;
}

void ldc(int arg) {
  VMContext *ctx = getVMContext();
  (void)ctx;
  pushCPUStack(arg);
  return;
}

void ldi(int arg) {
  VMContext *ctx = getVMContext();
  (void)arg;
  int addr = popCPUStack();
  if (checkError(ctx, &addr, NULL, NULL, NULL))
    return;
  pushCPUStack(ctx->memory[addr]);
  return;
}

void sti(int arg) {
  VMContext *ctx = getVMContext();
  (void)arg;
  int addr = popCPUStack();
  if (checkError(ctx, &addr, NULL, NULL, NULL))
    return;
  int item = popCPUStack();
  ctx->memory[addr] = item;
  return;
}
