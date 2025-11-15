#pragma once

#include "types.h"

typedef struct {
  unsigned int flags;
  int memory[INIT_MEMORY_SIZE];
  InstructionGroup inst_group[TOTAL_OPCODE_GROUPS];
  ChangeList changes;
  Statistics stat;
  Stack cpu_stack;
} VMContext;

VMContext *getVMContext(void);
void initVMContext(void);
void freeVMContext(void);
