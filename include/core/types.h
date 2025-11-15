#pragma once

#include "constants.h"

typedef struct {
  char name[MAX_LABEL_NAME_LEN];
  int addr;
} Label;

typedef struct {
  Label *labels;
  int count;
  int capacity;
} LabelList;

typedef struct {
  int core_idx;
  char label_name[MAX_LABEL_NAME_LEN];
} Patch;

typedef struct {
  Patch *patches;
  int count;
  int capacity;
} PatchList;

typedef struct {
  int index;
  int block;
  int offset;
  int size;
} Symbol;

typedef struct {
  Symbol *symbols;
  int count;
  int capacity;
} SymbolList;
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
