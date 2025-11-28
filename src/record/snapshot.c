#include "core/vm_context.h"
#include "record/record.h"

#include <stdlib.h>
#include <string.h>

void expandSnapshotList() {
  VMContext *ctx = getVMContext();
  ctx->snapshot_list.snapshot_capacity *= 2;
  ctx->snapshot_list.snapshot_list =
      realloc(ctx->snapshot_list.snapshot_list,
              sizeof(Snapshot) * ctx->snapshot_list.snapshot_capacity);
  return;
}

void makeSnapshot() {
  VMContext *ctx = getVMContext();

  if (ctx->snapshot_list.snapshot_count >= ctx->snapshot_list.snapshot_capacity)
    expandSnapshotList();

  int idx = ctx->snapshot_list.snapshot_count;
  memcpy(ctx->snapshot_list.snapshot_list[idx].memory, ctx->memory,
         sizeof(int) * INIT_MEMORY_SIZE);
  memcpy(ctx->snapshot_list.snapshot_list[idx].cpu_stack, ctx->cpu_stack.items,
         sizeof(int) * INIT_CPU_STACK_CAPACITY);

  ctx->snapshot_list.snapshot_list[idx].cpu_top = ctx->cpu_stack.top;
  ctx->snapshot_list.snapshot_list[idx].cs = ctx->cs;
  ctx->snapshot_list.snapshot_list[idx].pc = ctx->pc;
  ctx->snapshot_list.snapshot_list[idx].ds = ctx->ds;
  ctx->snapshot_list.snapshot_list[idx].ss = ctx->ss;
  ctx->snapshot_list.snapshot_list[idx].sp = ctx->sp;
  ctx->snapshot_list.snapshot_list[idx].bp = ctx->bp;
  ctx->snapshot_list.snapshot_list[idx].flags = ctx->flags;
  ctx->snapshot_list.snapshot_count++;
}

void restoreSnapshot(const Snapshot *snap) {
  VMContext *ctx = getVMContext();

  memcpy(ctx->memory, snap->memory, sizeof(int) * INIT_MEMORY_SIZE);
  memcpy(ctx->cpu_stack.items, snap->cpu_stack,
         sizeof(int) * INIT_CPU_STACK_CAPACITY);

  ctx->cpu_stack.top = snap->cpu_top;
  ctx->cs = snap->cs;
  ctx->pc = snap->pc;
  ctx->ds = snap->ds;
  ctx->ss = snap->ss;
  ctx->sp = snap->sp;
  ctx->bp = snap->bp;
  ctx->flags = snap->flags;
}