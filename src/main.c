#include "cli/cli.h"

int main(int argc, char *argv[]) {
  if (argc == 3) {
    runAsCLI(argv[1], argv[2]);
  }

  return 0;
}
