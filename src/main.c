#include "cli/cli.h"
#include "core/vm_context.h"
#include "gui/gui.h"

int main(int argc, char *argv[]) {
  initVMContext();
  VMContext *ctx = getVMContext();

  if (argc == 3) {
    ctx->run_mode = CLI;
    runAsCLI(argv[1], argv[2]);
  } else {
    ctx->run_mode = GUI;
    runAsGUI(argc, argv);
  }
}
