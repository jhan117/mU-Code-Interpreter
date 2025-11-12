#include "test.h"
#include <stdio.h>

int main(void) {
  test_initVMContext();
  test_freeVMContext();
  printf("\nAll vm_context tests passed successfully!\n");
  return 0;
}