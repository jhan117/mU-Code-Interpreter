#include "test.h"
#include <stdio.h>

int main(void) {
  testInitVMContext();
  testFreeVMContext();
  printf("\nAll vm_context tests passed successfully!\n");
  return 0;
}