#include "assemble.h"

#include "core/opcode.h"
#include <string.h>

static OpInfo op_table[] = {
    // 프로그램 구성 명령
    {"bgn", -1, 1, OPERAND_NUMBER},
    {"sym", -1, 3, OPERAND_NUMBER},
    {"end", -1, 0, OPERAND_NONE},
    {"nop", -1, 0, OPERAND_NONE},
    // 함수 정의 및 호출
    {"proc", OP_PROC, 1, OPERAND_NUMBER},
    {"ret", OP_RET, 0, OPERAND_NONE},
    {"ldp", OP_LDP, 0, OPERAND_NONE},
    {"push", OP_PUSH, 0, OPERAND_NONE},
    {"call", OP_CALL, 1, OPERAND_LABEL},
    // 흐름 제어
    {"ujp", OP_UJP, 1, OPERAND_LABEL},
    {"tjp", OP_TJP, 1, OPERAND_LABEL},
    {"fjp", OP_FJP, 1, OPERAND_LABEL},
    // 데이터 이동 연산자
    {"lod", OP_LOD, 2, OPERAND_BLOCK_OFFSET},
    {"lda", OP_LDA, 2, OPERAND_BLOCK_OFFSET},
    {"ldc", OP_LDC, 1, OPERAND_NUMBER},
    {"str", OP_STR, 2, OPERAND_BLOCK_OFFSET},
    {"ldi", OP_LDI, 0, OPERAND_NONE},
    {"sti", OP_STI, 0, OPERAND_NONE},
    // 이항 연산자
    {"gt", OP_GT, 0, OPERAND_NONE},
    {"lt", OP_LT, 0, OPERAND_NONE},
    {"ge", OP_GE, 0, OPERAND_NONE},
    {"le", OP_LE, 0, OPERAND_NONE},
    {"eq", OP_EQ, 0, OPERAND_NONE},
    {"ne", OP_NE, 0, OPERAND_NONE},
    {"and", OP_AND, 0, OPERAND_NONE},
    {"or", OP_OR, 0, OPERAND_NONE},
    {"add", OP_ADD, 0, OPERAND_NONE},
    {"sub", OP_SUB, 0, OPERAND_NONE},
    {"mult", OP_MULT, 0, OPERAND_NONE},
    {"div", OP_DIV, 0, OPERAND_NONE},
    {"mod", OP_MOD, 0, OPERAND_NONE},
    // 단항 연산자
    {"not", OP_NOT, 0, OPERAND_NONE},
    {"neg", OP_NEG, 0, OPERAND_NONE},
};

const OpInfo *findOpInfo(const char *name) {
  for (size_t i = 0; i < sizeof(op_table) / sizeof(op_table[0]); i++) {
    if (strcmp(op_table[i].name, name) == 0)
      return &op_table[i];
  }
  return NULL;
}