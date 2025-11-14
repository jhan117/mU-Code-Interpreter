#include "test.h"
#include <stdlib.h>
#include <string.h>

//================메모리================
static int *memory;
int *getMemory(void) { return memory; }

void initMemory(void) {
  freeMemory();
  memory = malloc(sizeof(int) * INIT_MEMORY_SIZE);
}

void freeMemory(void) {
  if (memory) {
    free(memory);
    memory = NULL;
  }
}

//================CPU 스택=================
static CPUStack cpu_stack;
CPUStack *getCPUStack(void) { return &cpu_stack; }

void initCPUStack(void) {
  freeCPUStack();
  cpu_stack.capacity = INIT_CPU_STACK_CAPACITY;
  cpu_stack.top = 0;
  cpu_stack.items = malloc(sizeof(int) * INIT_CPU_STACK_CAPACITY);
}

void freeCPUStack(void) {
  if (cpu_stack.items) {
    free(cpu_stack.items);
    cpu_stack.items = NULL;
  }
  memset(&cpu_stack, 0, sizeof(CPUStack));
}

static inline int isFull(void) { return cpu_stack.top == cpu_stack.capacity; }
static inline int isEmpty(void) { return cpu_stack.top == 0; }

int pushCPUStack(int item) {
  if (isFull()) {
    VMContext *ctx = getVMContext();
    ctx->flags |= ERR_CPU_STACK_OVERFLOW;
    return 0;
  }

  cpu_stack.items[cpu_stack.top++] = item;
  return 1;
}

int popCPUStack() {
  if (isEmpty()) {
    VMContext *ctx = getVMContext();
    ctx->flags |= ERR_CPU_STACK_UNDERFLOW;
    return 0;
  }

  return cpu_stack.items[--cpu_stack.top];
}
