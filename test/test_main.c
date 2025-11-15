#include "test.h"
#include <stdio.h>

int main(void) {
  printf("=====stack_operations 테스트=====\n");
  testPushCPUStack();
  testPopCPUStack();
  printf("All stack_operations tests passed successfully\n\n");

  printf("=====vm_context 테스트=====\n");
  testInitVMContext();
  testFreeVMContext();
  printf("All vm_context tests passed successfully!\n\n");
  return 0;
}