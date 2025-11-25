#include "core/vm_context.h"

#include "assembler/assemble_utils.h" // addSystemLabel()
#include "core/inst.h"
#include <stdlib.h>
#include <string.h>

static VMContext ctx;
VMContext *getVMContext() { return &ctx; }

// VMContext 초기화
void initVMContext() {
  freeVMContext();

  ctx.cs = ctx.pc = ctx.ds = ctx.ss = ctx.sp = ctx.bp = 0;
  ctx.code_len = 0;
  ctx.g_var_cnt = 0;

  ctx.source_map.line = malloc(sizeof(int) * INIT_LINE_CAPACITY);
  ctx.source_map.len = 0;
  ctx.source_map.capacity = INIT_LINE_CAPACITY;
  ctx.label_list.labels = malloc(sizeof(Label) * INIT_LIST_CAPACITY);
  ctx.label_list.count = 0;
  ctx.label_list.capacity = INIT_LIST_CAPACITY;
  addSystemLabel();
  ctx.patch_list.patches = malloc(sizeof(Patch) * INIT_LIST_CAPACITY);
  ctx.patch_list.count = 0;
  ctx.patch_list.capacity = INIT_LIST_CAPACITY;
  ctx.symbol_list.symbols = malloc(sizeof(Symbol) * INIT_LIST_CAPACITY);
  ctx.symbol_list.count = 0;
  ctx.symbol_list.capacity = INIT_LIST_CAPACITY;
  ctx.func_list.items = malloc(sizeof(FuncInfo) * INIT_LIST_CAPACITY);
  ctx.func_list.count = 0;
  ctx.func_list.capacity = INIT_LIST_CAPACITY;
  ctx.call_patch_list.patches = malloc(sizeof(CallPatch) * INIT_LIST_CAPACITY);
  ctx.call_patch_list.count = 0;
  ctx.call_patch_list.capacity = INIT_LIST_CAPACITY;

  // 실행 모드 초기화
  ctx.run_mode = CLI;
  // 에러 플래그 초기화
  ctx.flags = 0;

  // 메모리 초기화
  memset(ctx.memory, 0, sizeof(int) * INIT_MEMORY_SIZE);

  // 상태 변화 리스트 초기화
  ctx.changes.change_list = malloc(sizeof(Change *) * INIT_LIST_CAPACITY);
  ctx.changes.list_count = 0;
  ctx.changes.list_size = INIT_LIST_CAPACITY;

  // 통계 정보 초기화
  for (int i = 0; i < OPCODE_MAX; i++) {
    ctx.stat.inst_run_count[i] = 0;
    ctx.stat.inst_use_count[i] = 0;
  }
  ctx.stat.memory_access_count = 0;

  // 명령어 그룹 테이블 초기화
  initInstGroup();
  for (int i = 0; i < TOTAL_OPCODE_GROUPS; i++) {
    ctx.inst_group[i].group_id = i;
  }

  ctx.cpu_stack.capacity = INIT_CPU_STACK_CAPACITY;
  ctx.cpu_stack.top = 0;
  ctx.cpu_stack.items = malloc(sizeof(int) * INIT_CPU_STACK_CAPACITY);
}

// VMContext 해제
void freeVMContext() {
  if (ctx.source_map.line) {
    free(ctx.source_map.line);
    ctx.source_map.line = NULL;
  }

  if (ctx.label_list.labels) {
    free(ctx.label_list.labels);
    ctx.label_list.labels = NULL;
  }
  if (ctx.patch_list.patches) {
    free(ctx.patch_list.patches);
    ctx.patch_list.patches = NULL;
  }
  if (ctx.symbol_list.symbols) {
    free(ctx.symbol_list.symbols);
    ctx.symbol_list.symbols = NULL;
  }
  if (ctx.func_list.items) {
    free(ctx.func_list.items);
    ctx.func_list.items = NULL;
  }
  if (ctx.call_patch_list.patches) {
    free(ctx.call_patch_list.patches);
    ctx.call_patch_list.patches = NULL;
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