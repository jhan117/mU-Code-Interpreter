#pragma once

#include "types.h"

void initMemory(void);
void freeMemory(void);

void initCPUStack(void);
void freeCPUStack(void);
int pushCPUStack(int);
int popCPUStack(void);