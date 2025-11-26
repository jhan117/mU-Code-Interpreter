#include "core/vm_context.h"
#include "runner/runner.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Snapshot snapshot;
static SnapshotList snapshot_list;
SnapshotList *getSnapshotList() { return &snapshot_list; }

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

void initSnapshotList() {
  freeSnapshotList();
  snapshot_list.snapshot_list =
      malloc(sizeof(Snapshot) * INIT_SNAPSHOT_LIST_CAPACITY);
  snapshot_list.snapshot_count = 0;
  snapshot_list.snapshot_capacity = INIT_SNAPSHOT_LIST_CAPACITY;
}

void freeSnapshotList() {
  if (snapshot_list.snapshot_list) {
    free(snapshot_list.snapshot_list);
  }
  memset(&snapshot_list, 0, sizeof(SnapshotList));
}

void expandSnapshotList() {
  snapshot_list.snapshot_capacity *= 2;
  snapshot_list.snapshot_list =
      realloc(snapshot_list.snapshot_list, snapshot_list.snapshot_capacity);
  return;
}

void makeSnapshot() {
  VMContext *ctx = getVMContext();

  if (snapshot_list.snapshot_count >= snapshot_list.snapshot_capacity)
    expandSnapshotList();
  snapshot_list.snapshot_count++;
  memcpy(snapshot_list.snapshot_list->memory, ctx->memory,
         sizeof(int) * INIT_MEMORY_SIZE);
  memcpy(snapshot_list.snapshot_list->cpu_stack, ctx->cpu_stack.items,
         sizeof(int) * INIT_CPU_STACK_CAPACITY);

  int idx = snapshot_list.snapshot_count;
  snapshot_list.snapshot_list[idx].cpu_top = ctx->cpu_stack.top;
  snapshot_list.snapshot_list[idx].cs = ctx->cs;
  snapshot_list.snapshot_list[idx].pc = ctx->pc;
  snapshot_list.snapshot_list[idx].ds = ctx->ds;
  snapshot_list.snapshot_list[idx].ss = ctx->ss;
  snapshot_list.snapshot_list[idx].sp = ctx->sp;
  snapshot_list.snapshot_list[idx].bp = ctx->bp;
  snapshot_list.snapshot_list[idx].flags = ctx->flags;
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
    addNewChange(HD_NUM_BP, 0, ctx->bp);
  }
  if (snapshot.pc != ctx->pc) {
    addNewChange(HD_NUM_PC, 0, ctx->pc);
    snapshot.pc = ctx->pc;
  }
  if (snapshot.sp != ctx->sp) {
    addNewChange(HD_NUM_SP, 0, ctx->sp);
    snapshot.sp = ctx->sp;
  }
  ctx->changes.list_count++;

  if (ctx->changes.list_count >= ctx->changes.list_size) {
    expandChangeList();
  }

  return;
}