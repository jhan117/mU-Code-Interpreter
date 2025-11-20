#include "runner.h"
#include "test.h"

#include <assert.h>
#include <stdio.h>

static int stub_exec_called;
static int stub_exec_inst;

static void stubExecInst(int inst) {
  stub_exec_called = 1;
  stub_exec_inst = inst;
}

static int changeExists(Change *node, int hdware_num, int addr, int value) {
  while (node) {
    if (node->hdware_num == hdware_num && node->addr == addr &&
        node->new_value == value) {
      return 1;
    }
    node = node->next;
  }
  return 0;
}

void testRunnerStep(void) {
  initVMContext();
  VMContext *ctx = getVMContext();

  // step 테스트
  int initial_pc = 10;
  ctx->pc = initial_pc;

  int inst = (0 << 29) | (3 << 26) | 0x15;
  ctx->memory[initial_pc] = inst;

  void (*originalExec)(int) = ctx->inst_group[0].execInst;
  stub_exec_called = 0;
  stub_exec_inst = 0;
  ctx->inst_group[0].execInst = stubExecInst;

  step();

  assert(stub_exec_called == 1);
  assert(stub_exec_inst == inst);
  assert(ctx->pc == initial_pc + 1);
  assert(ctx->stat.memory_access_count == 1);

  ctx->inst_group[0].execInst = originalExec;

  printf("test : step() pass\n");
}

void testRunnerErrorExit(void) {
  initVMContext();
  VMContext *ctx = getVMContext();

  // 실행 중 오류 발생 테스트
  ctx->flags = ERR_INVALID_PC;
  ctx->bp = 0;

  assert(runner() == -1);

  printf("test : runner() error exit pass\n");
}

void testRunnerSaveChanges(void) {
  initVMContext();
  VMContext *ctx = getVMContext();

  // 변경 내용 저장 테스트
  ctx->ds = 10;
  ctx->ss = 20;
  ctx->sp = 15;
  ctx->bp = 30;
  ctx->pc = 5;

  initSnapshot();

  ctx->memory[16] = 111;
  ctx->memory[12] = 222;

  ctx->sp = 17;
  ctx->bp = 32;
  ctx->pc = 7;

  ctx->cpu_stack.top = 2;
  ctx->cpu_stack.items[0] = 333;
  ctx->cpu_stack.items[1] = 444;

  saveChanges();

  assert(ctx->changes.list_count == 1);

  Change *head = ctx->changes.change_list[0];
  assert(head != NULL);

  assert(changeExists(head, HD_NUM_MEMORY, 16, 111));
  assert(changeExists(head, HD_NUM_MEMORY, 12, 222));
  assert(changeExists(head, HD_NUM_CPU_STACK, 0, 333));
  assert(changeExists(head, HD_NUM_CPU_STACK, 1, 444));
  assert(changeExists(head, HD_NUM_BP, 0, 32));
  assert(changeExists(head, HD_NUM_PC, 0, 7));
  assert(changeExists(head, HD_NUM_SP, 0, 17));

  printf("test : saveChanges() pass\n");
}
