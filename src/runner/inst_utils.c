#include "core/vm_context.h"
#include "runner/inst.h"
#include "runner/runner.h"
#include <stdio.h>

int checkError(VMContext *ctx, const int *addr, const int *pc, const int *bp,
               const int *sp) {
  int result = 0;
  if (addr != NULL) {
    // 주소는 DS ~ MEM_END 범위 내에서
    int value = *addr;
    if ((ctx->ds > value) || (value > INIT_MEMORY_SIZE)) {
      ctx->flags |= ERR_INVALID_ADDR;
      result = 1;
    }
  }
  if (pc != NULL) {
    // PC는 CS ~ DS-1 범위 내에서
    int value = *pc;
    if ((ctx->cs > value) || (value >= ctx->ds)) {
      ctx->flags |= ERR_INVALID_PC;
      result = 1;
    }
  }
  if (bp != NULL) {
    // BP는
    // SS + 1(caller base와 return address 들어갈 공간) ~ MEM_END 범위내에서
    int value = *bp;
    if ((ctx->ss + 2 > value) || (value >= INIT_MEMORY_SIZE)) {
      ctx->flags |= ERR_INVALID_BP;
      result = 1;
    }
  }
  if (sp != NULL) {
    // SP는 SS ~ BP 범위 내에서
    int value = *sp;
    if (value < ctx->ss) {
      ctx->flags |= ERR_STACK_OVERFLOW;
      result = 1;
    }
    if (value >= ctx->bp) {
      ctx->flags |= ERR_STACK_UNDERFLOW;
      result = 1;
    }
  }
  return result;
}
