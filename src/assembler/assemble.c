#include "assemble.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int isNumber(const char *s) {
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

static void freeOperands(char *operands[], int count) {
  for (int j = 0; j < count; j++)
    free(operands[j]);
}

static int returnError(int code, char *operands[], int count, int line,
                       const char *msg) {
  if (msg)
    printf("[ERROR] Line %d: %s\n", line, msg);
  freeOperands(operands, count);
  return code;
}

static inline int encodeInst(int opcode, int operand_val) {
  return ((opcode & 0x3F) << 26) | (operand_val & 0x03FFFFFF);
}

int assemble(char **lines, int line_count) {
  VMContext *ctx = getVMContext();
  int addr = 0;

  for (int i = 0; i < line_count; i++) {
    char *line = lines[i];
    if (line[0] == '%' || line[0] == '\0')
      continue;

    char label[MAX_LABEL_LEN];
    char cmd[MAX_CMD_LEN];
    char *operands[MAX_OPERANDS];
    int operand_count = 0;

    int r = parseLine(line, label, cmd, operands, &operand_count);
    if (r != ASSEMBLE_OK)
      return returnError(r, operands, operand_count, i + 1, "parse error");

    if (label[0]) {
      if (findLabel(label) != -1)
        return returnError(ASSEMBLE_ERR_LABEL_DUP, operands, operand_count,
                           i + 1, "duplicate label");
      addLabel(label, addr);
    }

    if (!cmd[0]) {
      freeOperands(operands, operand_count);
      continue;
    }

    // 명령어 조회
    const OpInfo *info = findOpInfo(cmd);
    if (!info)
      return returnError(ASSEMBLE_ERR_INVALID_FORMAT, operands, operand_count,
                         i + 1, "unknown command");

    // 피연산자 개수 검증
    if (operand_count != info->operand_count)
      return returnError(ASSEMBLE_ERR_ARG_COUNT, operands, operand_count, i + 1,
                         "wrong operand count");

    if (info->opcode < 0) {
      if (strcmp(cmd, "bgn") == 0) {
        ctx->symbols.count = 0;
      } else if (strcmp(cmd, "sym") == 0) {
        if (!isNumber(operands[0]) || !isNumber(operands[1]) ||
            !isNumber(operands[2]))
          return returnError(ASSEMBLE_ERR_ARG_TYPE, operands, operand_count,
                             i + 1, "invalid operand type");
        int block = atoi(operands[0]);
        int offset = atoi(operands[1]);
        int size = atoi(operands[2]);
        if (findSymbol(block, offset) != -1)
          return returnError(ASSEMBLE_ERR_VAR_DUP, operands, operand_count,
                             i + 1, "duplicate symbol");
        addSymbol(block, offset, size);
      } else if (strcmp(cmd, "end") == 0 || strcmp(cmd, "nop") == 0) {
        // 나중에 필요하면 추가
      }
      freeOperands(operands, operand_count);
      continue;
    }

    int operand_val = 0;
    switch (info->operand_type) {
    case OPERAND_NONE:
      operand_val = 0;
      break;

    case OPERAND_NUMBER:
      if (!isNumber(operands[0]))
        return returnError(ASSEMBLE_ERR_ARG_TYPE, operands, operand_count,
                           i + 1, "invalid operand type");
      operand_val = atoi(operands[0]);
      break;

    case OPERAND_LABEL: {
      if (isNumber(operands[0]))
        return returnError(ASSEMBLE_ERR_ARG_TYPE, operands, operand_count,
                           i + 1, "invalid operand type");
      int la = findLabel(operands[0]);
      if (la < 0 && !isSystemLabel(operands[0])) {
        addPatch(addr, operands[0]);
        operand_val = 0;
      } else {
        operand_val = la;
      }
      break;
    }

    case OPERAND_BLOCK_OFFSET: {
      if (!isNumber(operands[0]) || !isNumber(operands[1]))
        return returnError(ASSEMBLE_ERR_ARG_TYPE, operands, operand_count,
                           i + 1, "invalid operand type");
      int block = atoi(operands[0]);
      int offset = atoi(operands[1]);

      int idx = findSymbol(block, offset);
      if (idx < 0) {
        return returnError(ASSEMBLE_ERR_VAR_UNDEF, operands, operand_count,
                           i + 1, "symbol not found");
      }
      operand_val = idx;
      break;
    }
    }

    // 메모리 경계 체크
    if (addr < 0 || addr >= INIT_MEMORY_SIZE)
      return returnError(ASSEMBLE_ERR_MEMORY, operands, operand_count, i + 1,
                         "memory address out of range");

    ctx->memory[addr++] = encodeInst(info->opcode, operand_val);
    freeOperands(operands, operand_count);
  }
  return applyPatches();
}
