#pragma once

#include "assembler/assemble_error.h"
#include "core/vm_context.h"
#include "record/record.h"
#include "runner/u_code_instructions/u_code.h"

// 어셈블러 테스트
typedef struct {
  AssembleError expected;
  char *lines[INIT_MEMORY_SIZE + 1];
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
