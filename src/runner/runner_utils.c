#include "core/vm_context.h"
#include "runner.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Snapshot snapshot;

void updateSymbols() {
  VMContext *ctx = getVMContext();

  for (int i = 0; i < ctx->symbols.count; i++) {
    if (ctx->symbols.symbols[i].block != 0) {
      // 주소를 계산
    }
  }
}

void initSnapshot() {
  VMContext *ctx = getVMContext();
  memcpy(snapshot.memory, ctx->memory, sizeof(int) * INIT_MEMORY_SIZE);
  memcpy(snapshot.cpu_stack, ctx->cpu_stack.items,
         sizeof(int) * INIT_CPU_STACK_CAPACITY);
  snapshot.cpu_top = ctx->cpu_stack.top;
  snapshot.cs = ctx->cs;
  snapshot.pc = ctx->pc;
  snapshot.ds = ctx->ds;
  snapshot.ss = ctx->ss;
  snapshot.sp = ctx->sp;
  snapshot.bp = ctx->bp;
  snapshot.flags = ctx->flags;
}

void addNewChange(int hdware_num, int addr, int new_value) {
  VMContext *ctx = getVMContext();

  Change *head = ctx->changes.change_list[ctx->changes.list_count];

  Change *new = malloc(sizeof(Change));
  new->addr = addr;
  new->hdware_num = hdware_num;
  new->new_value = new_value;
  new->next = head;

  head = new;
  ctx->changes.change_list[ctx->changes.list_count] = head;
}

void expandChangeList() {
  VMContext *ctx = getVMContext();
  ctx->changes.list_size *= 2;
  ctx->changes.change_list = realloc(ctx->changes.change_list,
                                     sizeof(Change *) * ctx->changes.list_size);
}

void saveChanges() {
  VMContext *ctx = getVMContext();

  int start = snapshot.sp + 1 <= ctx->sp + 1 ? snapshot.sp + 1 : ctx->sp + 1;
  int end = snapshot.bp > ctx->bp ? snapshot.bp : ctx->bp;
  ctx->changes.change_list[ctx->changes.list_count] = NULL;
  // stack영역
  for (int i = start; i <= end; i++) {
    if (snapshot.memory[i] != ctx->memory[i]) {
      addNewChange(HD_NUM_MEMORY, i, ctx->memory[i]);
      snapshot.memory[i] = ctx->memory[i];
    }
  }
  // data 영역
  start = ctx->ds;
  end = ctx->ss - 1;
  for (int i = start; i <= end; i++) {
    if (snapshot.memory[i] != ctx->memory[i]) {
      addNewChange(HD_NUM_MEMORY, i, ctx->memory[i]);
      snapshot.memory[i] = ctx->memory[i];
    }
  }
  // cpu 스택
  start = snapshot.cpu_top - 2 < ctx->cpu_stack.top - 2
              ? snapshot.cpu_top - 2
              : ctx->cpu_stack.top - 2;

  end = snapshot.cpu_top - 1 > ctx->cpu_stack.top - 1 ? snapshot.cpu_top - 1
                                                      : ctx->cpu_stack.top - 1;

  if (start < 0)
    start = 0;
  if (end < 0)
    end = 0;
  for (int i = start; i <= end; i++) {
    if (snapshot.cpu_stack[i] != ctx->cpu_stack.items[i]) {
      addNewChange(HD_NUM_CPU_STACK, i, ctx->cpu_stack.items[i]);
      snapshot.cpu_stack[i] = ctx->cpu_stack.items[i];
      snapshot.cpu_top = ctx->cpu_stack.top;
    }
  }
  // 레지스터
  if (snapshot.bp != ctx->bp) {
    snapshot.bp = ctx->bp;
    addNewChange(HD_NUM_BP, NULL, ctx->bp);
  }
  if (snapshot.pc != ctx->pc) {
    addNewChange(HD_NUM_PC, NULL, ctx->pc);
    snapshot.pc = ctx->pc;
  }
  if (snapshot.sp != ctx->sp) {
    addNewChange(HD_NUM_SP, NULL, ctx->sp);
    snapshot.sp = ctx->sp;
  }
  ctx->changes.list_count++;

  if (ctx->changes.list_count >= ctx->changes.list_size) {
    expandChangeList();
  }

  return;
}