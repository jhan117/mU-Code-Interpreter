#include "assembler/assemble_utils.h"

#include <ctype.h>
#include <string.h>

int isNumber(const char *s) {
  if (!s || !*s)
    return 0;
  if (*s == '+' || *s == '-')
    s++;
  while (*s) {
    if (!isdigit((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

AssembleError validOperands(const OpInfo *info, char *operands[], int count,
                            int *operand_val, int addr, int src_idx) {
  // 개수 확인
  if (count != info->operand_count)
    return ASSEMBLE_ERR_ARG_COUNT;

  // 타입 확인
  switch (info->operand_type) {
  case OPERAND_NONE:
    *operand_val = 0;
    break;

  case OPERAND_NUMBER:
    if (!isNumber(operands[0]))
      return ASSEMBLE_ERR_ARG_TYPE;
    *operand_val = atoi(operands[0]);
    break;

  case OPERAND_LABEL: {
    if (isNumber(operands[0]))
      return ASSEMBLE_ERR_ARG_TYPE;

    int label_addr = findLabel(operands[0]);
    if (label_addr == LABEL_NOT_FOUND) {
      addPatch(addr, src_idx, operands[0]);
    } else {
      *operand_val = label_addr;
    }
    break;
  }

  case OPERAND_BLOCK_OFFSET: {
    if (!isNumber(operands[0]) || !isNumber(operands[1]))
      return ASSEMBLE_ERR_ARG_TYPE;

    int block = atoi(operands[0]);
    int offset = atoi(operands[1]);

    int idx = findSymbol(block, offset);
    if (idx == SYMBOL_NOT_FOUND)
      return ASSEMBLE_ERR_VAR_UNDEF;
    else {
      *operand_val = idx;
    }
    break;
  }
  }
  return ASSEMBLE_ERR_NONE;
}