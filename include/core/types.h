#pragma once

#include "constants.h"

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
  int core_idx;
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