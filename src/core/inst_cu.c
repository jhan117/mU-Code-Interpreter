#include "core/inst.h"
#include "core/stack_operations.h"
#include "core/vm_context.h"
#include "runner.h"
#include <stdio.h>

static int nextBP;

void proc(int arg) {
  VMContext *ctx = getVMContext();
  ctx->sp -= arg;
  if (checkError(ctx, NULL, NULL, NULL, ctx->sp))
    return;
  return;
}

void ret(int arg) {
  VMContext *ctx = getVMContext();
  ctx->sp = ctx->bp;
  ctx->pc = ctx->memory[ctx->bp - 1];
  ctx->bp = ctx->memory[ctx->bp];
  if (checkError(ctx, NULL, ctx->pc, ctx->bp, NULL))
    return;
  return;
}

void ldp(int arg) {
  VMContext *ctx = getVMContext();
  nextBP = ctx->bp;
  ctx->bp = ctx->bp - 2;
  if (checkError(ctx, NULL, NULL, ctx->bp, NULL))
    return;
  return;
}

void push(int arg) {
  VMContext *ctx = getVMContext();
  int item = popCPUStack();
  ctx->memory[ctx->sp--] = item;
  if (checkError(ctx, NULL, NULL, NULL, ctx->sp))
    return;
  return;
}

void call(int arg) {
  VMContext *ctx = getVMContext();
  if (checkError(ctx, NULL, arg, NULL, NULL))
    return;
  if (arg >= 0) {
    ctx->memory[nextBP - 1] = ctx->pc;
    ctx->memory[nextBP] = ctx->bp;
    ctx->bp = nextBP;
    if (checkError(ctx, NULL, NULL, ctx->bp, NULL))
      return;
    ctx->pc = arg;
    // TODO: 변수 테이블 주소 계산
    return;
  } else {
    // TODO: 시스템 함수 호출
  }
  return;
}

void ujp(int arg) {
  VMContext *ctx = getVMContext();
  ctx->pc = arg;
  if (checkError(ctx, NULL, ctx->pc, NULL, NULL))
    return;
  return;
}

void tjp(int arg) {
  VMContext *ctx = getVMContext();
  if (popCPUStack()) {
    ctx->pc = arg;
    if (checkError(ctx, NULL, ctx->pc, NULL, NULL))
      return;
  }
  return;
}

void fjp(int arg) {
  VMContext *ctx = getVMContext();
  if (!popCPUStack()) {
    ctx->pc = arg;
    if (checkError(ctx, NULL, ctx->pc, NULL, NULL))
      return;
  }
  return;
}

void lod(int arg) {
  VMContext *ctx = getVMContext();
  int addr = ctx->symbols.symbols[arg].offset;
  if (checkError(ctx, addr, NULL, NULL, NULL))
    return;
  pushCPUStack(ctx->memory[addr]);
  return;
}

void lda(int arg) {
  VMContext *ctx = getVMContext();
  int addr = ctx->symbols.symbols[arg].offset;
  if (checkError(ctx, addr, NULL, NULL, NULL))
    return;
  pushCPUStack(ctx->memory[addr]);
  return;
}

void ldc(int arg) {
  VMContext *ctx = getVMContext();
  pushCPUStack(arg);
  return;
}

void str(int arg) {
  VMContext *ctx = getVMContext();
  int addr = ctx->symbols.symbols[arg].offset;
  if (checkError(ctx, addr, NULL, NULL, NULL))
    return;
  int item = popCPUStack();
  ctx->memory[addr] = item;
  return;
}

void ldi(int arg) {
  VMContext *ctx = getVMContext();
  int addr = popCPUStack();
  if (checkError(ctx, addr, NULL, NULL, NULL))
    return;
  pushCPUStack(ctx->memory[addr]);
  return;
}

void sti(int arg) {
  VMContext *ctx = getVMContext();
  int addr = popCPUStack();
  if (checkError(ctx, addr, NULL, NULL, NULL))
    return;
  int item = popCPUStack();
  ctx->memory[addr] = item;
  return;
}
