#include "core/instruction.h"

#include "core/opcode.h"
#include <string.h>

static const OpInfo op_table[] = {
    // 프로그램 구성 명령
    {"bgn", -1, 1, OPERAND_NUMBER, 0},
    {"sym", -1, 3, OPERAND_NUMBER, 0},
    {"end", OP_RET, 0, OPERAND_NONE, 2}, // return으로 취급
    {"nop", -1, 0, OPERAND_NONE, 0},
    // 함수 정의 및 호출
    {"proc", OP_PROC, 1, OPERAND_NUMBER, 0},
    {"ret", OP_RET, 0, OPERAND_NONE, 2},
    {"ldp", OP_LDP, 0, OPERAND_NONE, 0},
    {"push", OP_PUSH, 0, OPERAND_NONE, 1},
    {"call", OP_CALL, 1, OPERAND_LABEL, 2},
    // 흐름 제어
    {"ujp", OP_UJP, 1, OPERAND_LABEL, 0},
    {"tjp", OP_TJP, 1, OPERAND_LABEL, 0},
    {"fjp", OP_FJP, 1, OPERAND_LABEL, 0},
    // 데이터 이동 연산자
    {"lod", OP_LOD, 2, OPERAND_BLOCK_OFFSET, 1},
    {"lda", OP_LDA, 2, OPERAND_BLOCK_OFFSET, 1},
    {"ldc", OP_LDC, 1, OPERAND_NUMBER, 0},
    {"str", OP_STR, 2, OPERAND_BLOCK_OFFSET, 1},
    {"ldi", OP_LDI, 0, OPERAND_NONE, 1},
    {"sti", OP_STI, 0, OPERAND_NONE, 1},
    // 이항 연산자
    {"gt", OP_GT, 0, OPERAND_NONE, 0},
    {"lt", OP_LT, 0, OPERAND_NONE, 0},
    {"ge", OP_GE, 0, OPERAND_NONE, 0},
    {"le", OP_LE, 0, OPERAND_NONE, 0},
    {"eq", OP_EQ, 0, OPERAND_NONE, 0},
    {"ne", OP_NE, 0, OPERAND_NONE, 0},
    {"and", OP_AND, 0, OPERAND_NONE, 0},
    {"or", OP_OR, 0, OPERAND_NONE, 0},
    {"add", OP_ADD, 0, OPERAND_NONE, 0},
    {"sub", OP_SUB, 0, OPERAND_NONE, 0},
    {"mult", OP_MULT, 0, OPERAND_NONE, 0},
    {"div", OP_DIV, 0, OPERAND_NONE, 0},
    {"mod", OP_MOD, 0, OPERAND_NONE, 0},
    // 단항 연산자
    {"not", OP_NOT, 0, OPERAND_NONE, 0},
    {"neg", OP_NEG, 0, OPERAND_NONE, 0},
    {"dup", OP_DUP, 0, OPERAND_NONE, 0}};

static const int op_table_count = sizeof(op_table) / sizeof(op_table[0]);

const OpInfo *getOpInfo(int *count) {
  *count = op_table_count;
  return &op_table;
}

const OpInfo *findOpInfoByName(const char *name) {
  for (int i = 0; i < op_table_count; i++) {
    if (strcmp(op_table[i].name, name) == 0)
      return &op_table[i];
  }
  return NULL;
}

const OpInfo *findOpInfoByOpcode(int opcode) {
  for (int i = 0; i < op_table_count; i++) {
    if (opcode == op_table[i].opcode)
      return &op_table[i];
  }
  return NULL;
}

int encodeInst(int opcode, int operand_val) {
  int opGroup = (opcode / 10) & 0x7;
  int opGroupIdx = (opcode % 10) & 0x7;
  int operand = operand_val & 0x03FFFFFF;

  return (opGroup << 29) | (opGroupIdx << 26) | operand;
}

int patchInst(int old_inst, int new_addr) {
  return (old_inst & 0xFC000000) | (new_addr & 0x03FFFFFF);
}

void decodeInst(int old_inst, int *op_group, int *op_group_idx, int *operand) {
  if (op_group)
    *op_group = (old_inst >> 29) & 0x7;
  if (op_group_idx)
    *op_group_idx = (old_inst >> 26) & 0x7;
  if (operand) {
    *operand = old_inst & 0x03FFFFFF;

    // 음수 변환
    if (*operand & (1 << 25))
      *operand |= ~0x03FFFFFF;
  }
  return;
}
