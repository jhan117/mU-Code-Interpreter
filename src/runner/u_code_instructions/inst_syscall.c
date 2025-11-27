
#include "core/vm_context.h"
#include "record/record.h"
#include "runner/u_code_instructions/u_code.h"
#include <stdio.h>

void Read() {
  VMContext *ctx = getVMContext();
  int addr = ctx->sp + 1;
  ctx->sp--;
  if (checkError(ctx, &addr, NULL, NULL, &ctx->sp)) {
    return;
  }
  addr = ctx->memory[addr];
  int data = reqRead();
  ctx->memory[addr] = data;
  return;
}

void lf() {
  reqLf();
  ret(0);
  return;
}

void Write() {
  VMContext *ctx = getVMContext();
  int data = ctx->memory[ctx->sp + 1];
  ctx->sp--;
  if (checkError(ctx, NULL, NULL, NULL, &ctx->sp)) {
    return;
  }
  char data_s[33];
  sprintf(data_s, "%d ", data);
  reqWrite(data_s);
  return;
}