#include "cli/cli.h"
#include "core/vm_context.h"
#include "gui/gui.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  initVMContext();
  VMContext *ctx = getVMContext();

  if (argc >= 2 && strcmp(argv[1], "--benchmark") == 0) {
    if (argc < 3) {
      printf("[ERROR] --benchmark 옵션은 uco 파일 경로가 필요합니다.\n");
      return 1;
    }
    ctx->run_mode = CLI;
    runAsCLI(argv[2], NULL, 1);
    return 0;
  }

  if (argc == 3) {
    ctx->run_mode = CLI;
    runAsCLI(argv[1], argv[2], 0);
  } else {
    ctx->run_mode = GUI;
    runAsGUI(argc, argv);
  }

  return 0;
}
