#include "core/vm_context.h"

#include <stdlib.h>
#include <string.h>

static VMContext ctx;
VMContext *getVMContext(void) { return &ctx; }

static void addSystemLabel(const char *name, int addr) {
  LabelList *t = &getVMContext()->labels;

  strncpy(t->labels[t->count].name, name, MAX_LABEL_NAME_LEN - 1);
  t->labels[t->count].name[MAX_LABEL_NAME_LEN - 1] = '\0';
  t->labels[t->count].addr = addr;
  t->count++;
}

// VMContext 초기화/해제
void initVMContext(void) {
  // 기존 메모리 해제
  freeVMContext();

  // 레이블 리스트 초기화
  ctx.labels.labels = malloc(sizeof(Label) * INIT_LIST_CAPACITY);
  ctx.labels.count = 0;
  ctx.labels.capacity = INIT_LIST_CAPACITY;

  addSystemLabel("read", -1);
  addSystemLabel("write", -2);
  addSystemLabel("lf", -3);

  // 패치 리스트 초기화
  ctx.patches.patches = malloc(sizeof(Patch) * INIT_LIST_CAPACITY);
  ctx.patches.count = 0;
  ctx.patches.capacity = INIT_LIST_CAPACITY;

  // 심볼 리스트 초기화
  ctx.symbols.symbols = malloc(sizeof(Symbol) * INIT_LIST_CAPACITY);
  ctx.symbols.count = 0;
  ctx.symbols.capacity = INIT_LIST_CAPACITY;

  // 레지스터 초기화
  ctx.cs = ctx.pc = ctx.ds = ctx.ss = ctx.sp = ctx.bp = 0;
}

// VMContext 해제
void freeVMContext(void) {
  if (ctx.labels.labels) {
    free(ctx.labels.labels);
    ctx.labels.labels = NULL;
  }
  if (ctx.patches.patches) {
    free(ctx.patches.patches);
    ctx.patches.patches = NULL;
  }
  if (ctx.symbols.symbols) {
    free(ctx.symbols.symbols);
    ctx.symbols.symbols = NULL;
  }

  memset(&ctx, 0, sizeof(VMContext));
}