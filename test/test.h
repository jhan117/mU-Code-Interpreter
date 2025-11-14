#pragma once

#include "core/cpu_resources.h"
#include "core/vm_context.h"

void testInitMemory(void);
void testFreeMemory(void);

void testInitCPUStack(void);
void testFreeCPUStack(void);
void testPushCPUStack(void);
void testPopCPUStack(void);

void testInitVMContext(void);
void testFreeVMContext(void);