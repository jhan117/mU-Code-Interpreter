#include "test.h"

#include <assert.h>
#include <stdio.h>

void test_initCPUStack(void) {
  CPUStack *stack = malloc(sizeof(CPUStack));
  initCPUStack(stack, INIT_CPU_STACK_CAPACITY);

  assert(stack->capacity == INIT_CPU_STACK_CAPACITY);
  assert(stack->top == 0);
  assert(stack->items != NULL);

  printf("test_initCPUStack() pass\n");
}

void test_freeCPUStack(void) {
  CPUStack *stack = malloc(sizeof(CPUStack));
  initCPUStack(stack, INIT_CPU_STACK_CAPACITY);
  freeCPUStack(stack);

  assert(stack->capacity == 0);
  assert(stack->top == 0);
  assert(stack->items == NULL);

  for (size_t i = 0; i < sizeof(CPUStack); i++) {
    assert(((unsigned char *)stack)[i] == 0);
  }

  printf("test_freeCPUStack() pass\n");
}

void test_pushCPUStack(void) {}

void test_popCPUstack(void) {}