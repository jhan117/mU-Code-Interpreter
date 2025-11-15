#include "test.h"
#include <stdio.h>

int main(void) {
  int failures = 0;

  printf("\n===== Running assemble success tests... =====\n");
  failures += test_assemble_success();

  printf("\n===== Running assemble failure tests... =====\n");
  failures += test_assemble_failures();

  if (failures == 0)
    printf("\nAll tests passed!\n");
  else
    printf("\n%d test(s) failed\n", failures);

  return failures;
}