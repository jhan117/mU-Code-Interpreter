#include "test.h"

#include <assert.h>
#include <stdio.h>

// VMContext 초기화/해제
void testInitVMContext(void) {

  initVMContext();
  VMContext *ctx = getVMContext();
  // 에러 플래그 초기화 테스트
  assert(ctx->flags == 0);

  // 상태 변화 리스트 초기화 테스트
  assert(ctx->changes.change_list != NULL);
  assert(ctx->changes.list_count == 0);

  // 통계 정보 초기화 테스트
  for (int i = 0; i < OPCODE_MAX; i++) {
    assert(ctx->stat.inst_run_count[i] == 0);
    assert(ctx->stat.inst_use_count[i] == 0);
  }
  assert(ctx->stat.memory_access_count == 0);

  // 명령어 그룹 테이블 초기화 테스트
  for (int i = 0; i < TOTAL_OPCODE_GROUPS; i++) {
    assert(ctx->inst_group[i].execInst ==
           NULL); // 명령어 처리 함수 구현 후 변경 테스트
    assert(ctx->inst_group[i].group_id == i);
  }

  printf("test_initVMContext() pass\n");
}

// VMContext 해제
void testFreeVMContext(void) {

  initVMContext();
  VMContext *ctx = getVMContext();

  freeVMContext();

  assert(ctx->changes.change_list == NULL);

  for (size_t i = 0; i < sizeof(VMContext); i++) {
    assert(((unsigned char *)ctx)[i] == 0);
  }
  printf("test_freeVMContext() pass\n");
}