#include "core/inst.h"
#include "core/stack_operations.h"
#include "core/vm_context.h"
#include "runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OutputBuffer output_buffer;
OutputBuffer *getOutputBuffer() { return &output_buffer; }

void initOutBuffer() {
  freeOutBuffer();
  output_buffer.data = malloc(sizeof(char) * INIT_OUTPUT_BUF_CAPACITY);
  output_buffer.capacity = INIT_OUTPUT_BUF_CAPACITY;
  output_buffer.length = 0;
  if (output_buffer.data)
    output_buffer.data[0] = '\0';
  return;
}

void freeOutBuffer() {
  if (output_buffer.data) {
    free(output_buffer.data);
    output_buffer.data = NULL;
  }
  memset(&output_buffer, 0, sizeof(OutputBuffer));
  return;
}

void catString(const char *s) {
  int len = strlen(s);

  while (output_buffer.length + len + 1 > output_buffer.capacity) {
    expandOutBuffer();
  }

  memcpy(output_buffer.data + output_buffer.length, s, len);
  output_buffer.length += len;

  output_buffer.data[output_buffer.length] = '\0';
}

void expandOutBuffer() {
  output_buffer.capacity *= 2;
  output_buffer.data =
      realloc(output_buffer.data, sizeof(char) * output_buffer.capacity);
}

int reqWrite(const char *s) {
  VMContext *ctx = getVMContext();
  if (ctx->run_mode == CLI) {
    printf("%s", s);
  }
  if (ctx->run_mode == GUI) {
    // gui에 쓰기 요청
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
    // data = gui에 읽기 요청
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
    // gui에 줄바꿈 요청
  }
  catString("\n");
}

void lf() {
  reqLf();
  ret(0);
  return;
}
