#include "core/inst.h"
#include "core/vm_context.h"
#include "runner.h"
#include <stdio.h>

int checkError(VMContext *ctx, const int *addr, const int *pc, const int *bp,
               const int *sp) {
  int result = 0;
  if (addr != NULL) {
    int value = *addr;
    if ((ctx->ds > value) || (value > ctx->bp - 2)) {
      ctx->flags |= ERR_INVALID_ADDR;
      result = 1;
    }
  }
  if (pc != NULL) {
    int value = *pc;
    if ((ctx->cs > value) || (value >= ctx->ds)) {
      ctx->flags |= ERR_INVALID_PC;
      result = 1;
    }
  }
  if (bp != NULL) {
    int value = *bp;
    if ((ctx->ss + 2 > value) || (value >= INIT_MEMORY_SIZE)) {
      ctx->flags |= ERR_INVALID_BP;
      result = 1;
    }
  }
  if (sp != NULL) {
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

inline int decodeGroup(int inst) { return (inst >> 29) & 0x7; }

inline int decodeOpcode(int inst) { return (inst >> 26) & 0x7; }

inline int decodeArg(int inst) {
  int arg = inst & 0x03FFFFFF;
  if (arg & (1 << 25)) {
    arg |= ~0x03FFFFFF;
  }
  return arg;
}
