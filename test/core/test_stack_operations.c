#include "core/stack_operations.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void testPushCPUStack(void) {
  VMContext *ctx = getVMContext();
  initVMContext();
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