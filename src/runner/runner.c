#include "runner.h"
#include "core/inst.h"
#include "core/opcode.h"
#include "core/vm_context.h"
#include <stdio.h>

static const int opcode_base[TOTAL_OPCODE_GROUPS] = {OP_PROC, OP_UJP, OP_LOD,
                                                     OP_GT, OP_ADD};

static const int mem_access[OPCODE_MAX] = {
    [OP_RET] = 2, [OP_PUSH] = 1, [OP_CALL] = 2, [OP_LOD] = 1,
    [OP_LDA] = 1, [OP_STR] = 1,  [OP_LDI] = 1,  [OP_STI] = 1};

int step() {
  VMContext *ctx = getVMContext();
  int inst = ctx->memory[ctx->pc++];
  int group_code = decodeGroup(inst);
  int opcode = opcode_base[group_code] + decodeOpcode(inst);
  ctx->stat.memory_access_count += mem_access[opcode];
  ctx->inst_group[group_code].execInst(inst);
}

int runner() { VMContext *ctx = getVMContext(); }
