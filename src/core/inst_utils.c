#include "core/inst.h"
#include "core/vm_context.h"
#include "runner.h"
#include <stdio.h>

void updateSymbols() {
  VMContext *ctx = getVMContext();

  for (int i = 0; i < ctx->symbols.count; i++) {
    if (ctx->symbols.symbols[i].block != 0) {
      ctx->symbols.symbols[i].addr =
          ctx->bp - 2 - ctx->symbols.symbols[i].offset;
    }
  }
  return;
}

int checkError(VMContext *ctx, int addr, int pc, int bp, int sp) {
  int result = 0;
  if (addr != NULL) {
    if ((ctx->ds > addr) || (addr > ctx->bp - 2)) {
      ctx->flags |= ERR_INVALID_ADDR;
      result = 1;
    }
  }
  if (pc != NULL) {
    if ((ctx->cs > pc) || (pc >= ctx->ds)) {
      ctx->flags |= ERR_INVALID_PC;
      result = 1;
    }
  }
  if (bp != NULL) {
    if ((ctx->ss + 2 > bp) || (bp >= INIT_MEMORY_SIZE)) {
      ctx->flags |= ERR_INVALID_BP;
      result = 1;
    }
  }
  if (sp != NULL) {
    if (sp < ctx->ss) {
      ctx->flags |= ERR_STACK_OVERFLOW;
      result = 1;
    }
    if (sp >= ctx->bp) {
      ctx->flags |= ERR_STACK_UNDERFLOW;
      result = 1;
    }
  }
  return result;
}

inline int decodeGroup(int inst) { return (inst >> 29) & 0x7; }

inline int decodeOpcode(int inst) { return (inst >> 26) & 0x7; }

inline int decodeArg(int inst) { return inst & 0x03FFFFFF; }