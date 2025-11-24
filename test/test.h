#pragma once

#include "core/stack_operations.h"
#include "core/vm_context.h"

// 어셈블러 테스트
typedef struct {
  int id;
  char **lines;
  int count;
  int expected;
} TestCase;
int testAssembleSuccess();
int testAssembleFailures();

// 실행기 테스트
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
