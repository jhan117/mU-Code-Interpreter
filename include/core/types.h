#pragma once

#include "constants.h"

typedef struct Change {
  int hdware_num;
  int addr;
  int old_value;
  int new_value;
  struct Change *next;
} Change;

typedef struct {
  Change **change_list;
  int list_count;
} ChangeList;

typedef struct {
  int group_id;
  void (*execInst)(int);
} InstructionGroup;

typedef struct {
  int inst_use_count[OPCODE_MAX];
  int inst_run_count[OPCODE_MAX];
  int memory_access_count;
} Statistics;

typedef struct {
  int capacity;
  int top;
  int *items;
} Stack;