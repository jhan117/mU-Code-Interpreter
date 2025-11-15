#pragma once

#include "core/stack_operations.h"
#include "core/vm_context.h"

int testAssembleSuccess();
int testAssembleFailures();

void testPushCPUStack(void);
void testPopCPUStack(void);

void testInitVMContext(void);
void testFreeVMContext(void);
