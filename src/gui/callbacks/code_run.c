#include "gui/callbacks.h"

#include "assemble.h"
#include "gui/gui_context.h"
#include "gui/runner_dummy.h"

int find_nearest_snapshot_index(int step) {
  int lower = (step / SNAPSHOT_INTERVAL) * SNAPSHOT_INTERVAL;
  int upper = lower + SNAPSHOT_INTERVAL;

  if (step - lower < upper - step)
    return lower / SNAPSHOT_INTERVAL; // index
  else
    return upper / SNAPSHOT_INTERVAL; // index
}

void updateVM(int step) {
  SnapshotList *snap_list = getSnapshotList();
  int snap_index = find_nearest_snapshot_index(step);

  if (snap_index >= SNAPSHOT_COUNT)
    snap_index = SNAPSHOT_COUNT - 1;

  Snapshot snap = snap_list->snapshot_list[snap_index]; // 기준 snapshot 복사

  int steps_since_snapshot = step - snap_index * SNAPSHOT_INTERVAL;
  Change *head = NULL;
#ifdef USE_DUMMY
  head = dummy_changes.change_list[snap_index];
#endif // USE_DUMMY
  int applied = 0;

  printf("snap_idx = %d %d\n", snap_index, steps_since_snapshot);

  while (head && applied < steps_since_snapshot) {
    switch (head->hdware_num) {
    case HD_NUM_MEMORY:
      if (snap.memory[head->addr] != head->new_value) {
        snap.memory[head->addr] = head->new_value;
      }
      break;
    case HD_NUM_CPU_STACK:
      if (snap.cpu_stack[head->addr] != head->new_value) {
        snap.cpu_stack[head->addr] = head->new_value;
      }
      break;
    case HD_NUM_CS:
      if (snap.cs != head->new_value) {
        printf("%d, %d\n", snap.cs, head->new_value);
        snap.cs = head->new_value;
      }
      break;
    case HD_NUM_PC:
      if (snap.pc != head->new_value) {
        snap.pc = head->new_value;
      }
      break;
    case HD_NUM_DS:
      if (snap.ds != head->new_value) {
        snap.ds = head->new_value;
      }
      break;
    case HD_NUM_SS:
      if (snap.ss != head->new_value) {
        snap.ss = head->new_value;
      }
      break;
    case HD_NUM_SP:
      if (snap.sp != head->new_value) {
        snap.sp = head->new_value;
      }
      break;
    case HD_NUM_BP:
      if (snap.bp != head->new_value) {
        snap.bp = head->new_value;
      }
      break;
    }
    head = head->next;
    applied++;
  }

  updateRegTable(&snap);
  updateCPUStackView(&snap);
  updateMemoryView(&snap);
}

void freeLines(char **lines, int line_count) {
  if (!lines)
    return;

  for (int i = 0; i < line_count; i++) {
    free(lines[i]);
  }
  free(lines);
}

void onRun(GtkButton *button) {
  initVMContext();
  GuiContext *ctx = getGuiContext();
  ctx->is_run_done = 0;

  char **lines = NULL;
  int line_count = 0;
  if (!loadTableToUco(&lines, &line_count)) {
    // 경고창
    return;
  }

  int asm_res = assemble(lines, line_count);
  if (asm_res != ASSEMBLE_OK) {
    // 경고창
    freeLines(lines, line_count);
    return;
  }

  freeLines(lines, line_count);
  setAssembleView();

#ifdef USE_DUMMY
  initDummyVM();

  updateVM(ctx->current_step);
  updateLabelsView();
  updateSymbolsView();
  updateStatisticsBox();
#endif

  // run은 나중에
  // int run_res = runner();
  // if (run_res == -1) {
  //   // 경고창
  // initVMContext();
  //   return;
  // }

  ctx->is_run_done = 1;
}