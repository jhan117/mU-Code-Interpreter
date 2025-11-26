#include "assembler/assemble.h"

#include "assembler/assemble_utils.h"
#include "core/opcode.h" // OP_* 필요
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeOperands(char *operands[], int count) {
  for (int i = 0; i < count; i++)
    free(operands[i]);
}

static void pushSrcLine(int line) {
  SourceMap *source_map = &getVMContext()->source_map;

  if (source_map->len >= source_map->capacity) {
    source_map->capacity *= 2;
    source_map->line =
        realloc(source_map->line, sizeof(int) * source_map->capacity);
  }
  source_map->line[source_map->len++] = line;
}

AssembleError assemble(char **lines, int line_count) {
  VMContext *ctx = getVMContext();
  int addr = 0;
  FuncInfo *current_func = NULL;
  int param_cnt = 0;

  for (int i = 0; i < line_count; i++) {
    char *line = lines[i];

    // 주석, 빈줄 제외
    char *line_ptr = line;
    while (isspace(*line_ptr))
      line_ptr++;
    if (*line_ptr == '%' || !*line_ptr)
      continue;

    // 메모리 경계 체크
    if (addr < 0 || addr >= INIT_MEMORY_SIZE)
      return returnError(ASSEMBLE_ERR_MEMORY, i + 1);

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
        current_func = addFunc("global", addr);

        if (!current_func) {
          freeOperands(operands, operand_count);
          return returnError(ASSEMBLE_ERR_MEMORY, i + 1);
        }
        current_func->func_block = 1;
        continue;
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
        const char *name = findLabelByAddr(addr - 1);
        if (name) {
          FuncInfo *func = findFuncByName(name);
          if (func) {
            func->func_block = block;
          }
        }
        continue;
      } else if (strcmp(info->name, "nop") == 0) {
        continue;
      }
    }

    // operands 확인
    int operand_val = 0;
    AssembleError valid_operand_res =
        validOperands(info, operands, operand_count, &operand_val, addr, i + 1);
    if (valid_operand_res != ASSEMBLE_ERR_NONE) {
      freeOperands(operands, operand_count);
      return returnError(valid_operand_res, i + 1);
    }

    // 매개변수 확인
    switch (info->opcode) {
    case OP_PROC: {
      if (current_func) {
        return returnError(ASSEMBLE_ERR_RETURN, i + 1);
      }

      current_func = addFunc(label, addr);
      if (!current_func) {
        freeOperands(operands, operand_count);
        return returnError(ASSEMBLE_ERR_MEMORY, i + 1);
      }
      break;
    }
    case OP_RET: {
      if (!current_func) {
        freeOperands(operands, operand_count);
        return returnError(ASSEMBLE_ERR_PROC, i + 1);
      }

      current_func->end_addr = addr;
      current_func->is_start = 0;

      CallPatch *patch = findCallPatchByName(current_func->name);
      if (patch) {
        current_func->param_cnt = patch->param_cnt;
        operand_val = patch->param_cnt;
      }

      param_cnt = 0;
      current_func = NULL;
      break;
    }
    case OP_LDP:
      param_cnt = 0;
      break;
    case OP_PUSH:
      if (current_func && param_cnt < MAX_ARGS) {
        param_cnt++;
      }
      break;
    case OP_CALL: {
      if (isSystemLabel(operands[0])) {
        param_cnt = 0;
        break;
      }

      FuncInfo *func_info = findFuncByName(operands[0]);
      if (func_info) {
        func_info->param_cnt = param_cnt;
      } else {
        AssembleError patch_res = addCallPatch(operands[0], param_cnt);
        if (patch_res != ASSEMBLE_ERR_NONE) {
          freeOperands(operands, operand_count);
          return returnError(patch_res, i + 1);
        }
      }

      param_cnt = 0;
      break;
    }
    default:
      break;
    }

    ctx->stat.inst_use_count[info->opcode]++;
    ctx->memory[addr++] = encodeInst(info->opcode, operand_val);
    pushSrcLine(i);
    freeOperands(operands, operand_count);
  }

  if (current_func) {
    return returnError(ASSEMBLE_ERR_RETURN, line_count);
  }

  ctx->code_len = addr;

  AssembleError patch_res = applyPatches();
  if (patch_res != ASSEMBLE_ERR_NONE) {
    return patch_res;
  }

  AssembleError call_patch_res = applyCallPatch();
  if (call_patch_res != ASSEMBLE_ERR_NONE) {
    return call_patch_res;
  }

  applySymbolOffset();

  return ASSEMBLE_ERR_NONE;
}

char *printAssembleRes() {
  VMContext *ctx = getVMContext();

  int buf_size = ctx->code_len * LINE_BUFFER_LEN;
  char *result = malloc(buf_size);
  if (!result)
    return NULL;
  result[0] = '\0';

  for (int i = 0; i < ctx->code_len; i++) {
    int op_group = 0;
    int op_group_idx = 0;
    int operand = 0;

    decodeInst(ctx->memory[i], &op_group, &op_group_idx, &operand);

    int opcode = op_group * 10 + op_group_idx;

    char line[LINE_BUFFER_LEN];
    const OpInfo *info = findOpInfoByOpcode(opcode);
    if (info->operand_count == 0)
      snprintf(line, LINE_BUFFER_LEN, "%04d: opcode=%d\n", i, opcode);
    else
      snprintf(line, LINE_BUFFER_LEN, "%04d: opcode=%d operand=%d\n", i, opcode,
               operand);

    strncat(result, line, buf_size - strlen(result) - 1);
  }

  return result;
}