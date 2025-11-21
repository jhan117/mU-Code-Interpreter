#pragma once

#include "core/stack_operations.h"
#include "core/vm_context.h"

int testAssembleSuccess();
int testAssembleFailures();

void testRunnerStep(void);
void testRunnerErrorExit(void);
void testRunnerSaveChanges(void);
void testRunnerSnapshotListInit(void);
void testRunnerSnapshotAfterStep(void);

void testPushCPUStack(void);
void testPopCPUStack(void);

void testInitVMContext(void);
void testFreeVMContext(void);

void testFunctionInstructions(void);
void testBranchInstructions(void);
void testDataMovementInstructions(void);
void testArithmeticInstructions(void);
void testComparisonInstructions(void);
void testLogicalInstructions(void);
void testDecodeArgSignExtension(void);
