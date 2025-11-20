#include "core/inst.h"
#include "core/stack_operations.h"
#include "core/vm_context.h"
#include "runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *out_buffer;
static int out_buffer_capacity;
static int out_buffer_len;

void initOutBuffer() {
  freeOutBuffer();
  out_buffer = malloc(sizeof(char) * INIT_OUTPUT_BUF_CAPACITY);
  out_buffer[0] = '\0';
  out_buffer_capacity = INIT_OUTPUT_BUF_CAPACITY;
  out_buffer_len = 0;
  return;
}

void freeOutBuffer() {
  if (out_buffer) {
    free(out_buffer);
    out_buffer = NULL;
    out_buffer_capacity = 0;
    out_buffer_len = 0;
  }
  return;
}

void catString(const char *s) {
  int len = strlen(s);

  while (out_buffer_len + len + 1 > out_buffer_capacity) {
    expandOutBuffer();
  }

  memcpy(out_buffer + out_buffer_len, s, len);
  out_buffer_len += len;

  out_buffer[out_buffer_len] = '\0';
}

void expandOutBuffer() {
  out_buffer_capacity *= 2;
  out_buffer = realloc(out_buffer, sizeof(char) * out_buffer_capacity);
}

int reqWrite(const char *s) {
  VMContext *ctx = getVMContext();
  if (ctx->run_mode == CLI) {
    printf("%s", s);
  }
  if (ctx->run_mode == GUI) {
    // callWrite() gui 요청
  }
  catString(s);

  return 1;
}

void Write() {
  int data = popCPUStack();
  char data_s[33];
  sprintf(data_s, "%d", data);
  reqWrite(data_s);
  ret(0);
  return;
}

int reqRead() {
  VMContext *ctx = getVMContext();
  int data;
  if (ctx->run_mode == CLI) {
    scanf("%d", &data);
  }
  if (ctx->run_mode == GUI) {
    // data = callRead() gui 요청
  }
  return data;
}

void Read() {
  VMContext *ctx = getVMContext();
  int addr = popCPUStack();
  if (checkError(ctx, &addr, NULL, NULL, NULL)) {
    ret(0);
    return;
  }
  int data = reqRead();
  ctx->memory[addr] = data;
  ret(0);
  return;
}

void reqLf() {
  VMContext *ctx = getVMContext();
  if (ctx->run_mode == CLI) {
    printf("\n");
  }
  if (ctx->run_mode == GUI) {
    // gui에 요청
  }
  catString("\n");
}

void lf() {
  reqLf();
  ret(0);
  return;
}
