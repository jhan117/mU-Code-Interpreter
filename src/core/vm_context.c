#include "core/vm_context.h"

#include <stdlib.h>

static VMContext ctx;
VMContext *getVMContext(void) { return &ctx; }

// VMContext 초기화/해제
void initVMContext(void) {
  // 기존 메모리 해제
  freeVMContext();

  // 에러 플래그 초기화
  ctx.flags = 0;

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
}

// VMContext 해제
void freeVMContext(void) {
  if (ctx.changes.change_list) {
    free(ctx.changes.change_list);
    ctx.changes.change_list = NULL;
  }

  memset(&ctx, 0, sizeof(VMContext));
}