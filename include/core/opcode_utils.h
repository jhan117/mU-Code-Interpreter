#pragma once

#include "core/constants.h"
#include "core/opcode.h"

typedef struct {
  const int *codes;
  const char *const *names;
  int count;
} OpcodeData;

const OpcodeData *getOpcodeData(void);
const char *getOpcodeName(int opcode);
int getOpcodeFromGroup(int group_code, int opcode_index);
int getOpcodeFromInst(int inst);
