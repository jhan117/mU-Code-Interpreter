#include "runner/runner.h"
#include "core/instruction.h"
#include "core/opcode.h"
#include "core/vm_context.h"
#include "record/record.h"

#include <stdio.h>
#include <stdlib.h>

void step(void) {
  VMContext *ctx = getVMContext();
  int inst = ctx->memory[ctx->pc++];
  int group_code;
  int g_idx;
  decodeInst(inst, &group_code, &g_idx, NULL);
  int opcode = group_code * 10 + g_idx;
  ctx->stat.inst_run_count[opcode]++;
  const OpInfo *op = findOpInfoByOpcode(opcode);
  if (op)
    ctx->stat.memory_access_count += op->mem_access;
  ctx->inst_group[group_code].execInst(inst);
  return;
}

char *formatRunError(int line) {
  VMContext *ctx = getVMContext();
  if (ctx->flags == 0)
    return;

  const char *errors[7];
  int count = 0;

  if (ctx->flags & ERR_INVALID_ADDR)
    errors[count++] = "Invalid address access";
  if (ctx->flags & ERR_INVALID_PC)
    errors[count++] = "Invalid program counter";
  if (ctx->flags & ERR_INVALID_BP)
    errors[count++] = "Invalid base pointer";
  if (ctx->flags & ERR_STACK_OVERFLOW)
    errors[count++] = "Stack overflow";
  if (ctx->flags & ERR_STACK_UNDERFLOW)
    errors[count++] = "Stack underflow";
  if (ctx->flags & ERR_CPU_STACK_OVERFLOW)
    errors[count++] = "CPU stack overflow";
  if (ctx->flags & ERR_CPU_STACK_UNDERFLOW)
    errors[count++] = "CPU stack underflow";

  int total_len = 0;
  for (int i = 0; i < count; i++) {
    if (i == 0)
      total_len +=
          snprintf(NULL, 0, "[ERROR] ucode Line %d: %s\n", line, errors[i]);
    else
      total_len +=
          snprintf(NULL, 0, "                             %s\n", errors[i]);
  }

  char *buf = malloc(total_len + 1);
  if (!buf)
    return NULL;
  buf[0] = '\0';

  for (int i = 0; i < count; i++) {
    char tmp[256];
    if (i == 0)
      snprintf(tmp, sizeof(tmp), "[ERROR] ucode Line %d: %s\n", line,
               errors[i]);
    else
      snprintf(tmp, sizeof(tmp), "                             %s\n",
               errors[i]);
    strncat(buf, tmp, total_len - strlen(buf));
  }

  return buf; // free 필수
}

void printRunError(int line) {
  char *msg = formatRunError(line);

  if (msg) {
    printf("%s\n", msg);
    free(msg);
  }
}

void readyToRun() {
  VMContext *ctx = getVMContext();
  ctx->cs = 0;
  ctx->ds = ctx->code_len;
  ctx->ss = ctx->code_len + ctx->g_var_cnt;
  ctx->pc = 0;

  int stack_top = INIT_MEMORY_SIZE - 1;
  ctx->bp = stack_top;
  ctx->sp = stack_top - 2;
  ctx->memory[stack_top - 1] = 0;
  ctx->memory[stack_top] = -1;
}

ErrorResult runner() {
  ErrorResult ok = {ERR_SRC_NONE, 0, -1};

  VMContext *ctx = getVMContext();
  readyToRun();
  initSnapshot();

  ctx->prev_pc = 0;
  while (1) {
    if (ctx->bp == -1) {
      printf("\n[INFO] runner stopped\n");
      return ok;
    }
    if (ctx->flags != 0) {
      printRunError(ctx->source_map.line[ctx->prev_pc]);
      return (ErrorResult){ERR_SRC_RUNNER, ctx->flags,
                           ctx->source_map.line[ctx->prev_pc]};
    }
    ctx->prev_pc = ctx->pc;
    step();
    saveChanges();
  }
}
