#pragma once

#include "constants.h"

typedef enum {
  OPERAND_NONE = 0,
  OPERAND_NUMBER,
  OPERAND_LABEL,
  OPERAND_BLOCK_OFFSET
} OperandType;

typedef struct {
  const char *name;
  int opcode;
  int operand_count;
  OperandType operand_type;
} OpInfo;

const OpInfo *findOpInfoByName(const char *name);
int encodeInst(int opcode, int operand_val);
int patchInst(int old_opcode, int new_addr);
void decodeInst(int old_inst, int *op_group, int *op_group_idx, int *operand);
int getOperand(int inst);