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

void ldp(int arg) {
  VMContext *ctx = getVMContext();
  (void)arg;
  ctx->memory[ctx->sp] = ctx->bp;
  ctx->bp = ctx->sp;
  ctx->sp = ctx->sp - 2;
  if (checkError(ctx, NULL, NULL, NULL, &ctx->sp))
    return;
  return;
}

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
    ctx->memory[ctx->bp - 1] = ctx->pc;
    if (checkError(ctx, NULL, &arg, &ctx->bp, NULL))
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

void lod(int arg) {
  VMContext *ctx = getVMContext();
  int addr = ctx->symbols.symbols[arg].addr;
  if (checkError(ctx, &addr, NULL, NULL, NULL))
    return;
  pushCPUStack(ctx->memory[addr]);
  return;
}

void lda(int arg) {
  VMContext *ctx = getVMContext();
  int addr = ctx->symbols.symbols[arg].addr;
  pushCPUStack(addr);
  return;
}

void ldc(int arg) {
  VMContext *ctx = getVMContext();
  (void)ctx;
  pushCPUStack(arg);
  return;
}

void str(int arg) {
  VMContext *ctx = getVMContext();
  int addr = ctx->symbols.symbols[arg].addr;
  if (checkError(ctx, &addr, NULL, NULL, NULL))
    return;
  int item = popCPUStack();
  ctx->memory[addr] = item;
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
