#include "assembler/assemble.h"

#include "assembler/assemble_utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeOperands(char *operands[], int count) {
  for (int i = 0; i < count; i++)
    free(operands[i]);
}

AssembleError assemble(char **lines, int line_count) {
  VMContext *ctx = getVMContext();
  int addr = 0;

  for (int i = 0; i < line_count; i++) {
    char *line = lines[i];

    // 주석, 빈줄 제외
    char *line_ptr = line;
    while (isspace(*line_ptr))
      line_ptr++;
    if (*line_ptr == '%' || !*line_ptr)
      continue;

    // 코드 파싱
    char label[MAX_LABEL_LEN];
    char opcode[MAX_OP_LEN];
    char *operands[MAX_OPERANDS];
    int operand_count = 0;

    AssembleError parse_res =
        parseLine(line, label, opcode, operands, &operand_count);
    if (parse_res != ASSEMBLE_ERR_NONE) {
      freeOperands(operands, operand_count);
      return returnError(parse_res, i + 1);
    }

    // 라벨 확인
    if (label[0]) {
      if (findLabel(label) != LABEL_NOT_FOUND) {
        freeOperands(operands, operand_count);
        return returnError(ASSEMBLE_ERR_LABEL_DUP, i + 1);
      }
      AssembleError label_res = addLabel(label, addr);
      if (label_res != ASSEMBLE_ERR_NONE) {
        freeOperands(operands, operand_count);
        return returnError(label_res, i + 1);
      }
    }

    // opcode 확인
    const OpInfo *info = findOpInfoByName(opcode);
    if (!info) {
      freeOperands(operands, operand_count);
      return returnError(ASSEMBLE_ERR_INVALID_FORMAT, i + 1);
    }
    // 어셈블러 전용 opcode 처리
    if (info->opcode < 0) {
      if (strcmp(info->name, "bgn") == 0) {
        if (!isNumber(operands[0])) {
          freeOperands(operands, operand_count);
          return returnError(ASSEMBLE_ERR_ARG_TYPE, i + 1);
        }
        ctx->g_var_cnt = atoi(operands[0]);
      } else if (strcmp(info->name, "sym") == 0) {
        if (!isNumber(operands[0]) || !isNumber(operands[1]) ||
            !isNumber(operands[2])) {
          freeOperands(operands, operand_count);
          return returnError(ASSEMBLE_ERR_ARG_TYPE, i + 1);
        }
        int block = atoi(operands[0]);
        int offset = atoi(operands[1]);
        int size = atoi(operands[2]);
        if (findSymbol(block, offset) != SYMBOL_NOT_FOUND) {
          freeOperands(operands, operand_count);
          return returnError(ASSEMBLE_ERR_VAR_DUP, i + 1);
        }
        AssembleError symbol_res = addSymbol(block, offset, size);
        if (symbol_res != ASSEMBLE_ERR_NONE) {
          freeOperands(operands, operand_count);
          return returnError(symbol_res, i + 1);
        }
      }
    }

    // operands 확인
    int operand_val = 0;
    AssembleError valid_operand_res =
        validOperands(info, operands, operand_count, &operand_val, i + 1);
    if (valid_operand_res != ASSEMBLE_ERR_NONE) {
      freeOperands(operands, operand_count);
      return returnError(valid_operand_res, i + 1);
    }

    // 메모리 경계 체크
    if (addr < 0 || addr >= INIT_MEMORY_SIZE) {
      freeOperands(operands, operand_count);
      return returnError(ASSEMBLE_ERR_MEMORY, i + 1);
    }

    ctx->memory[addr++] = encodeInst(info->opcode, operand_val);
    freeOperands(operands, operand_count);
  }
  ctx->code_len = addr;

  AssembleError patch_res = applyPatches();
  if (patch_res != ASSEMBLE_ERR_NONE) {
    return patch_res;
  }
  return ASSEMBLE_ERR_NONE;
}
