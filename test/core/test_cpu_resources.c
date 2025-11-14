#include "core/cpu_resources.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void testInitMemory(void) {
  initMemory();
  int *memory = getMemory();
  assert(memory != NULL);
  printf("test : initMemory() pass\n");
}

void testFreeMemory(void) {
  initMemory();
  freeMemory();
  int *memory = getMemory();
  assert(memory == NULL);

  printf("test : freeMemory() pass\n");
}

void testInitCPUStack(void) {
  CPUStack *stack = getCPUStack();
  freeCPUStack();
  initCPUStack();

  assert(stack->capacity == INIT_CPU_STACK_CAPACITY);
  assert(stack->top == 0);
  assert(stack->items != NULL);
  printf("test : initCPUStack() pass\n");
}

void testFreeCPUStack(void) {
  CPUStack *stack = getCPUStack();
  initCPUStack();
  freeCPUStack();

  assert(stack->capacity == 0);
  assert(stack->top == 0);
  assert(stack->items == NULL);

  for (size_t i = 0; i < sizeof(CPUStack); i++) {
    assert(((unsigned char *)stack)[i] == 0);
  }

  printf("test : freeCPUStack() pass\n");
}

void testPushCPUStack(void) {
  VMContext *ctx = getVMContext();
  initVMContext();
  initCPUStack();
  for (int i = 0; i < INIT_CPU_STACK_CAPACITY; i++) {
    assert(pushCPUStack(i) == 1);
  }
  assert(pushCPUStack(100) == 0);
  assert(ctx->flags & ERR_CPU_STACK_OVERFLOW);

  printf("test : pushCPUStack(int) pass\n");
}

void testPopCPUStack(void) {
  VMContext *ctx = getVMContext();
  initVMContext();
  initCPUStack();
  for (int i = 0; i < INIT_CPU_STACK_CAPACITY; i++) {
    pushCPUStack(i);
  }
  for (int i = INIT_CPU_STACK_CAPACITY - 1; i >= 0; i--) {
    assert(popCPUStack() == i);
  }
  assert(popCPUStack() == 0);
  assert((ctx->flags & ERR_CPU_STACK_UNDERFLOW) == ERR_CPU_STACK_UNDERFLOW);

  printf("test : popCPUStack(int) pass\n");
}