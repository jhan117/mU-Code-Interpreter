#include "gui/runner_dummy.h"

#include <stdlib.h>

#ifdef USE_DUMMY

Snapshot dummy_snapshots[SNAPSHOT_COUNT] = {
    {.memory = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0},
     .cpu_stack = {0, 1, 2, 3, 4, 5, 6, 7},
     .cpu_top = 7,
     .cs = 0,
     .pc = 0,
     .ds = 0,
     .ss = 8,
     .sp = 7,
     .bp = 7,
     .flags = 0},
    {.memory = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 0, 0, 0, 0, 0},
     .cpu_stack = {7, 6, 5, 4, 3, 2, 1, 0},
     .cpu_top = 7,
     .cs = 1,
     .pc = 5,
     .ds = 1,
     .ss = 8,
     .sp = 6,
     .bp = 6,
     .flags = 0}};

Change dummy_change_storage[SNAPSHOT_COUNT * CHANGES_PER_SNAPSHOT];
ChangeList dummy_changes;

void initDummyVM(void) {
  // SnapshotList 초기화
  initSnapshotList();
  SnapshotList *snap_list = getSnapshotList();
  for (int i = 0; i < SNAPSHOT_COUNT; i++) {
    snap_list->snapshot_list[i] = dummy_snapshots[i];
  }

  dummy_changes.list_size = SNAPSHOT_COUNT;
  dummy_changes.list_count = SNAPSHOT_COUNT;
  dummy_changes.change_list =
      malloc(sizeof(Change *) * dummy_changes.list_size);

  dummy_changes.change_list[0] = NULL;

  for (int s = 1; s < SNAPSHOT_COUNT; s++) { // 1번부터 변경점
    Change *head = NULL;
    for (int c = 0; c < CHANGES_PER_SNAPSHOT; c++) {
      Change *ch = &dummy_change_storage[(s - 1) * CHANGES_PER_SNAPSHOT +
                                         c]; // s-1로 offset
      ch->hdware_num = HD_NUM_MEMORY + (c % 5);
      ch->addr = c + s * 10;
      ch->new_value = (s + 1) * 100 + c;
      ch->next = head;
      head = ch;
    }
    dummy_changes.change_list[s] = head;
  }
}

#endif // USE_DUMMY