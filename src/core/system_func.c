#include "core/stack_operations.h"
#include "core/vm_context.h"
#include "inst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reqWrite(const char *s) {
  VMContext *ctx = getVMContext();
  if (ctx->run_mode == CLI) {
    printf("%s", s);
  }
  if (ctx->run_mode == GUI) {
    // callWrite() gui 요청
  }
  // lst 파일 반영 위해 버퍼에 저장
}

void write() {
  VMContext *ctx = getVMContext();
  int data = popCPUStack();
  char data_s[33];
  sprintf(data_s, "%d", data);
  reqWrite(data_s);
  ret(NULL);
  return;
}

int reqRead() {
  VMContext *ctx = getVMContext();
  int data;
  if (ctx->run_mode == CLI) {
    scanf("%d", &data);
  }
  if (ctx->run_mode == GUI) {
    // callRead() gui 요청
  }
  // lst 파일 반영 위해 버퍼에 저장
  return data;
}

void write() {
  VMContext *ctx = getVMContext();
  int addr = popCPUStack();
  if (checkError(ctx, addr, NULL, NULL, NULL)) {
    ret(NULL);
    return;
  }
  int data = reqRead();
  ctx->memory[addr] = data;
  ret(NULL);
  return;
}