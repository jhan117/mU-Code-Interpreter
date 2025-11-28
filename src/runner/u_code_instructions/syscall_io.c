#include "core/vm_context.h"
#include "gui/gui_callbacks.h"
#include "record/record.h"

#include <stdio.h>

int reqWrite(const char *s) {
  VMContext *ctx = getVMContext();
  if (ctx->run_mode == CLI) {
    printf("%s", s);
  }
  if (ctx->run_mode == GUI) {
    // gui에 쓰기 요청
    guiIoWrite(s);
  }
  catString(s);

  return 1;
}

int reqRead() {
  VMContext *ctx = getVMContext();
  int data;
  if (ctx->run_mode == CLI) {
    scanf("%d", &data);
  }
  if (ctx->run_mode == GUI) {
    // data = gui에 읽기 요청
    data = guiIoRead();
  }
  return data;
}
void reqLf() {
  VMContext *ctx = getVMContext();
  if (ctx->run_mode == CLI) {
    printf("\n");
  }
  if (ctx->run_mode == GUI) {
    // gui에 줄바꿈 요청
    guiIoLf();
  }
  catString("\n");
}
