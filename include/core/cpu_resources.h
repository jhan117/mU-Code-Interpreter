#pragma once

#include "types.h"

int *getMemory(void);
void initMemory(void);
void freeMemory(void);

CPUStack *getCPUStack(void);
void initCPUStack(void);
void freeCPUStack(void);
int pushCPUStack(int);
int popCPUStack(void);