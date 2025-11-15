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
  // 에러 플래그 초기화
  ctx.flags = 0;

  // 메모리 초기화
  memset(ctx.memory, 0, sizeof(int) * INIT_MEMORY_SIZE);

  // 상태 변화 리스트 초기화
  ctx.changes.change_list = malloc(sizeof(Change *) * INIT_LIST_CAPACITY);
  ctx.changes.list_count = 0;

  // 통계 정보 초기화
  for (int i = 0; i < OPCODE_MAX; i++) {
    ctx.stat.inst_run_count[i] = 0;
    ctx.stat.inst_use_count[i] = 0;
  }
  ctx.stat.memory_access_count = 0;

  // 명령어 그룹 테이블 초기화
  for (int i = 0; i < TOTAL_OPCODE_GROUPS; i++) {
    ctx.inst_group[i].execInst = NULL; // 명령어 처리 함수 구현 후 변경
    ctx.inst_group[i].group_id = i;
  }

  ctx.cpu_stack.capacity = INIT_CPU_STACK_CAPACITY;
  ctx.cpu_stack.top = 0;
  ctx.cpu_stack.items = malloc(sizeof(int) * INIT_CPU_STACK_CAPACITY);
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

  if (ctx.changes.change_list) {
    free(ctx.changes.change_list);
    ctx.changes.change_list = NULL;
  }
  if (ctx.cpu_stack.items) {
    free(ctx.cpu_stack.items);
    ctx.cpu_stack.items = NULL;
  }
  memset(&ctx, 0, sizeof(VMContext));
}