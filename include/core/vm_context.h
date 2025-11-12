#pragma once

#include "types.h"

typedef struct {
  unsigned int flags;
  InstructionGroup inst_group[TOTAL_OPCODE_GROUPS];
  ChangeList changes;
  Statistics stat;

} VMContext;

VMContext *getVMContext(void);
void initVMContext(void);
void freeVMContext(void);
