#include "cli/cli.h"
#include "gui/gui.h"

int main(int argc, char *argv[]) {
  if (argc == 3) {
    runAsCLI(argv[1], argv[2]);
  }

  runAsGUI(argc, argv);

  return 0;
}
