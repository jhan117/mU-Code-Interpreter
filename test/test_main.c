#include "test.h"
#include <stdio.h>

int main(void) {
  int failures = 0;

  printf("=====stack_operations 테스트=====\n");
  testPushCPUStack();
  testPopCPUStack();
  printf("All stack_operations tests passed successfully\n\n");

  printf("=====vm_context 테스트=====\n");
  testInitVMContext();
  testFreeVMContext();
  printf("All vm_context tests passed successfully!\n\n");

  // assembler test는 실패 횟수 반환함
  printf("\n===== Running assemble success tests... =====\n");
  failures += testAssembleSuccess();

  printf("\n===== Running assemble failure tests... =====\n");
  failures += testAssembleFailures();

  if (failures == 0)
    printf("\nAll tests passed!\n");
  else
    printf("\n%d test(s) failed\n", failures);

  return failures;
}