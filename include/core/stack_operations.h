#pragma once

#include "types.h"

void initCPUStack(void);
void freeCPUStack(void);
int pushCPUStack(int);
int popCPUStack(void);