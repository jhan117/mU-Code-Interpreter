#include "test.h"
#include <stdio.h>

int main(void) {

  testInitMemory();
  testFreeMemory();
  testInitCPUStack();
  testFreeCPUStack();
  testPushCPUStack();
  testPopCPUStack();
  printf("All cpu_resources tests passed successfully\n\n");

  testInitVMContext();
  testFreeVMContext();
  printf("All vm_context tests passed successfully!\n\n");
  return 0;
}