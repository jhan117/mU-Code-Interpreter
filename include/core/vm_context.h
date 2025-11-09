#pragma once

#include "types.h"

typedef struct {
  int cs;
  int pc;
  int ds;
  int ss;
  int sp;
  int bp;

  LabelList labels;
  PatchList patches;
  SymbolList symbols;
} VMContext;

VMContext *getVMContext(void);
void initVMContext(void);
void freeVMContext(void);