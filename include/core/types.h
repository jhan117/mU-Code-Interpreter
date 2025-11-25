#pragma once

#include "constants.h"

typedef struct {
  int *line;
  int capacity;
  int len;
} SourceMap;

typedef struct {
  char name[MAX_LABEL_LEN];
  int addr;
} Label;

typedef struct {
  Label *labels;
  int count;
  int capacity;
} LabelList;

typedef struct {
  int code_idx;
  int src_idx;
  char label_name[MAX_LABEL_LEN];
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

typedef struct {
  int func_block;
  int start_addr;
  int end_addr;
  char name[MAX_LABEL_LEN];
  int param_cnt;
  int is_start;
} FuncInfo;

typedef struct {
  FuncInfo *items;
  int count;
  int capacity;
} FuncList;

typedef struct {
  char func_name[MAX_LABEL_LEN];
  int param_cnt;
} CallPatch;

typedef struct {
  CallPatch *patches;
  int count;
  int capacity;
} CallPatchList;

typedef struct Change {
  int hdware_num;
  int addr;
  int new_value;
  struct Change *next;
} Change;

typedef struct {
  Change **change_list;
  int list_count;
  int list_size;
} ChangeList;

typedef struct {
  int memory[INIT_MEMORY_SIZE];
  int cpu_stack[INIT_CPU_STACK_CAPACITY];
  int cpu_top;
  int cs;
  int pc;
  int ds;
  int ss;
  int sp;
  int bp;
  int flags;
} Snapshot;

typedef struct {
  Snapshot *snapshot_list;
  int snapshot_count;
  int snapshot_capacity;
} SnapshotList;

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
