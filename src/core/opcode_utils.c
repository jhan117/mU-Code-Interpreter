#include "core/opcode_utils.h"

#include <stddef.h>

static const int opcode_codes[] = {
    OP_PROC, OP_RET, OP_LDP, OP_PUSH, OP_CALL, OP_UJP, OP_TJP, OP_FJP,
    OP_LOD,  OP_LDA, OP_LDC, OP_STR,  OP_LDI,  OP_STI, OP_GT,  OP_LT,
    OP_GE,   OP_LE,  OP_EQ,  OP_NE,   OP_AND,  OP_OR,  OP_ADD, OP_SUB,
    OP_MULT, OP_DIV, OP_MOD, OP_NOT,  OP_NEG};

static const char *const opcode_names[OPCODE_MAX] = {
    [OP_PROC] = "proc", [OP_RET] = "ret", [OP_LDP] = "ldp", [OP_PUSH] = "push",
    [OP_CALL] = "call", [OP_UJP] = "ujp", [OP_TJP] = "tjp", [OP_FJP] = "fjp",
    [OP_LOD] = "lod",   [OP_LDA] = "lda", [OP_LDC] = "ldc", [OP_STR] = "str",
    [OP_LDI] = "ldi",   [OP_STI] = "sti", [OP_GT] = "gt",   [OP_LT] = "lt",
    [OP_GE] = "ge",     [OP_LE] = "le",   [OP_EQ] = "eq",   [OP_NE] = "ne",
    [OP_AND] = "and",   [OP_OR] = "or",   [OP_ADD] = "add", [OP_SUB] = "sub",
    [OP_MULT] = "mult", [OP_DIV] = "div", [OP_MOD] = "mod", [OP_NOT] = "not",
    [OP_NEG] = "neg"};

static const int opcode_group_base[TOTAL_OPCODE_GROUPS] = {
    OP_PROC, OP_UJP, OP_LOD, OP_GT, OP_ADD};

static const OpcodeData opcode_data = {
    opcode_codes, opcode_names, sizeof(opcode_codes) / sizeof(opcode_codes[0])};

const OpcodeData *getOpcodeData(void) { return &opcode_data; }

const char *getOpcodeName(int opcode) {
  if (opcode < 0 || opcode >= OPCODE_MAX)
    return NULL;
  return opcode_names[opcode];
}

int getOpcodeFromGroup(int group_code, int opcode_index) {
  if (group_code < 0 || group_code >= TOTAL_OPCODE_GROUPS)
    return -1;
  int opcode = opcode_group_base[group_code] + opcode_index;
  if (opcode < 0 || opcode >= OPCODE_MAX)
    return -1;
  return opcode;
}

int getOpcodeFromInst(int inst) {
  int group_code = (inst >> 29) & 0x7;
  int opcode_index = (inst >> 26) & 0x7;
  return getOpcodeFromGroup(group_code, opcode_index);
}
