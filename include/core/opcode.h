#pragma once

typedef enum {
  // 함수 정의 및 호출 (5)
  OP_PROC = 00,
  OP_RET = 01,
  OP_LDP = 02,
  OP_PUSH = 03,
  OP_CALL = 04,

  // 흐름 제어 (3)
  OP_UJP = 10,
  OP_TJP = 11,
  OP_FJP = 12,

  // 데이터 이동 연산자 (6)
  OP_LOD = 20,
  OP_LDA = 21,
  OP_LDC = 22,
  OP_STR = 23,
  OP_LDI = 24,
  OP_STI = 25,

  // 이항 연산자 (13)
  OP_GT = 30,
  OP_LT = 31,
  OP_GE = 32,
  OP_LE = 33,
  OP_EQ = 34,
  OP_NE = 35,
  OP_AND = 36,
  OP_OR = 37,

  OP_ADD = 40,
  OP_SUB = 41,
  OP_MULT = 42,
  OP_DIV = 43,
  OP_MOD = 44,

  // 단항 연산자 (2)
  OP_NOT = 45,
  OP_NEG = 46,
} Opcode;