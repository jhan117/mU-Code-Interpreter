#include "core/vm_context.h"
#include "record/record.h"

#include <stdlib.h>
#include <string.h>

static Snapshot state_snapshot;

void initSnapshot() {
  VMContext *ctx = getVMContext();
  memcpy(state_snapshot.memory, ctx->memory, sizeof(int) * INIT_MEMORY_SIZE);
  memcpy(state_snapshot.cpu_stack, ctx->cpu_stack.items,
         sizeof(int) * INIT_CPU_STACK_CAPACITY);
  state_snapshot.cpu_top = ctx->cpu_stack.top;
  state_snapshot.cs = ctx->cs;
  state_snapshot.pc = ctx->pc;
  state_snapshot.ds = ctx->ds;
  state_snapshot.ss = ctx->ss;
  state_snapshot.sp = ctx->sp;
  state_snapshot.bp = ctx->bp;
  state_snapshot.flags = ctx->flags;
  state_snapshot.prev_pc = ctx->prev_pc;
}

void expandChangeList() {
  VMContext *ctx = getVMContext();
  ctx->changes.list_size *= 2;
  ctx->changes.change_list = realloc(ctx->changes.change_list,
                                     sizeof(Change *) * ctx->changes.list_size);
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

void saveChanges() {
  VMContext *ctx = getVMContext();

  if (ctx->changes.list_count % SNAPSHOT_INTERVAL == 0)
    makeSnapshot();

  int start = state_snapshot.sp + 1 <= ctx->sp + 1 ? state_snapshot.sp + 1
                                                   : ctx->sp + 1;
  int end = state_snapshot.bp > ctx->bp ? state_snapshot.bp : ctx->bp;
  ctx->changes.change_list[ctx->changes.list_count] = NULL;
  // stack영역
  for (int i = start; i <= end; i++) {
    if (state_snapshot.memory[i] != ctx->memory[i]) {
      addNewChange(HD_NUM_MEMORY, i, ctx->memory[i]);
      state_snapshot.memory[i] = ctx->memory[i];
    }
  }
  // data 영역
  start = ctx->ds;
  end = ctx->ss - 1;
  for (int i = start; i <= end; i++) {
    if (state_snapshot.memory[i] != ctx->memory[i]) {
      addNewChange(HD_NUM_MEMORY, i, ctx->memory[i]);
      state_snapshot.memory[i] = ctx->memory[i];
    }
  }
  // cpu 스택
  start = state_snapshot.cpu_top - 2 < ctx->cpu_stack.top - 2
              ? state_snapshot.cpu_top - 2
              : ctx->cpu_stack.top - 2;

  end = state_snapshot.cpu_top - 1 > ctx->cpu_stack.top - 1
            ? state_snapshot.cpu_top - 1
            : ctx->cpu_stack.top - 1;

  if (start < 0)
    start = 0;
  if (end < 0)
    end = 0;
  if (state_snapshot.cpu_top != ctx->cpu_stack.top) {
    addNewChange(HD_NUM_CPU_TOP, 0, ctx->cpu_stack.top);
    state_snapshot.cpu_top = ctx->cpu_stack.top;
  }
  for (int i = start; i <= end; i++) {
    if (state_snapshot.cpu_stack[i] != ctx->cpu_stack.items[i]) {
      addNewChange(HD_NUM_CPU_STACK, i, ctx->cpu_stack.items[i]);
      state_snapshot.cpu_stack[i] = ctx->cpu_stack.items[i];
      state_snapshot.cpu_top = ctx->cpu_stack.top;
    }
  }
  // 레지스터
  if (state_snapshot.bp != ctx->bp) {
    state_snapshot.bp = ctx->bp;
    addNewChange(HD_NUM_BP, 0, ctx->bp);
  }
  if (state_snapshot.pc != ctx->pc) {
    addNewChange(HD_NUM_PC, 0, ctx->pc);
    state_snapshot.pc = ctx->pc;
  }
  if (state_snapshot.sp != ctx->sp) {
    addNewChange(HD_NUM_SP, 0, ctx->sp);
    state_snapshot.sp = ctx->sp;
  }
  if (state_snapshot.prev_pc != ctx->prev_pc) {
    addNewChange(HD_NUM_PREV_PC, 0, ctx->prev_pc);
    state_snapshot.prev_pc = ctx->prev_pc;
  }
  ctx->changes.list_count++;

  if (ctx->changes.list_count >= ctx->changes.list_size) {
    expandChangeList();
  }

  return;
}

void applyChanges(int start_step, int end_step) {
  VMContext *ctx = getVMContext();

  for (int s = start_step; s <= end_step; s++) {
    Change *c = ctx->changes.change_list[s];
    while (c) {
      switch (c->hdware_num) {
      case HD_NUM_MEMORY:
        ctx->memory[c->addr] = c->new_value;
        break;
      case HD_NUM_CPU_STACK:
        ctx->cpu_stack.items[c->addr] = c->new_value;
        break;
      case HD_NUM_CS:
        ctx->cs = c->new_value;
        break;
      case HD_NUM_PC:
        ctx->pc = c->new_value;
        break;
      case HD_NUM_DS:
        ctx->ds = c->new_value;
        break;
      case HD_NUM_SS:
        ctx->ss = c->new_value;
        break;
      case HD_NUM_SP:
        ctx->sp = c->new_value;
        break;
      case HD_NUM_BP:
        ctx->bp = c->new_value;
        break;
      case HD_NUM_PREV_PC:
        ctx->prev_pc = c->new_value;
        break;
      case HD_NUM_CPU_TOP:
        ctx->cpu_stack.top = c->new_value;
        break;
      }
      c = c->next;
    }
  }
}