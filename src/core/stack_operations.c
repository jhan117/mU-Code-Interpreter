#include "core/vm_context.h"
#include <stdlib.h>
#include <string.h>

static inline int isFull(void) {
  VMContext *ctx = getVMContext();
  return ctx->cpu_stack.top == ctx->cpu_stack.capacity;
}
static inline int isEmpty(void) {
  VMContext *ctx = getVMContext();
  return ctx->cpu_stack.top == 0;
}

// CPU 스택 push 연산
int pushCPUStack(int item) {
  VMContext *ctx = getVMContext();
  if (isFull()) {
    VMContext *ctx = getVMContext();
    ctx->flags |= ERR_CPU_STACK_OVERFLOW;
    return 0;
  }

  ctx->cpu_stack.items[ctx->cpu_stack.top++] = item;
  return 1;
}

// CPU 스택 pop 연산
int popCPUStack() {
  VMContext *ctx = getVMContext();
  if (isEmpty()) {
    VMContext *ctx = getVMContext();
    ctx->flags |= ERR_CPU_STACK_UNDERFLOW;
    return 0;
  }

  return ctx->cpu_stack.items[--ctx->cpu_stack.top];
}
