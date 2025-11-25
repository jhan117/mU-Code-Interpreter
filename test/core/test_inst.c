#include "core/inst.h"
#include "test.h"

#include <assert.h>
#include <stdio.h>

static VMContext *setupContext(void) {
  initVMContext();
  VMContext *ctx = getVMContext();
  ctx->cs = 0;
  ctx->ds = 2048;
  ctx->ss = 3000;
  ctx->bp = 4096;
  ctx->sp = 3500;
  ctx->pc = 256;
  ctx->flags = 0;
  return ctx;
}

static void setSymbol(int index, int block, int addr) {
  VMContext *ctx = getVMContext();
  ctx->symbol_list.symbols[index].index = index;
  ctx->symbol_list.symbols[index].block = block;
  if (block == GLOBAL_BLOCK)
    ctx->symbol_list.symbols[index].offset = addr - ctx->ds;
  else
    ctx->symbol_list.symbols[index].offset = ctx->bp - addr;
  ctx->symbol_list.symbols[index].size = 1;
  if (ctx->symbol_list.count <= index)
    ctx->symbol_list.count = index + 1;
}

static int runBinaryInst(void (*inst)(int), int lhs, int rhs) {
  setupContext();
  pushCPUStack(lhs);
  pushCPUStack(rhs);
  inst(0);
  int result = popCPUStack();
  freeVMContext();
  return result;
}

static int runUnaryInst(void (*inst)(int), int value) {
  setupContext();
  pushCPUStack(value);
  inst(0);
  int result = popCPUStack();
  freeVMContext();
  return result;
}

void testFunctionInstructions(void) {
  VMContext *ctx = setupContext();

  // proc 테스트
  int prev_sp = ctx->sp;
  proc(24);
  assert(ctx->sp == prev_sp - 24);
  assert((ctx->flags & (ERR_STACK_OVERFLOW | ERR_STACK_UNDERFLOW)) == 0);
  freeVMContext();

  // ldp 테스트 (no-op)
  ctx = setupContext();
  // 아무것도 안함
  freeVMContext();

  // ret 테스트
  ctx = setupContext();
  ctx->bp = 3500;
  ctx->memory[ctx->bp - 1] = 1500;
  ctx->memory[ctx->bp] = 3300;
  ret(0);
  assert(ctx->pc == 1500);
  assert(ctx->bp == 3300);
  assert(ctx->sp == 3500);
  freeVMContext();

  // push 테스트
  ctx = setupContext();
  ctx->sp = 3600;
  pushCPUStack(777);
  push(0);
  assert(ctx->memory[3600] == 777);
  assert(ctx->sp == 3599);
  freeVMContext();

  // call 테스트
  ctx = setupContext();
  ctx->pc = 600;
  ctx->bp = 3700;
  int call_prev_sp = ctx->sp;
  int call_prev_bp = ctx->bp;
  int call_prev_pc = ctx->pc;
  call(1024);
  assert(ctx->pc == 1024);
  assert(ctx->bp == call_prev_sp);
  assert(ctx->sp == call_prev_sp - 2);
  assert(ctx->memory[ctx->bp] == call_prev_bp);
  assert(ctx->memory[ctx->bp - 1] == call_prev_pc);
  freeVMContext();

  // proc 실패 테스트
  // stack 영역 부족
  ctx = setupContext();
  ctx->sp = ctx->ss + 5;
  proc(10);
  assert((ctx->flags & ERR_STACK_OVERFLOW) == ERR_STACK_OVERFLOW);
  freeVMContext();

  // ldp 오류 테스트
  ctx = setupContext();
  // 아무것도 안함
  freeVMContext();

  // bp 오류 테스트
  ctx = setupContext();
  ctx->bp = ctx->sp - 10;
  pushCPUStack(999);
  push(0);
  assert((ctx->flags & ERR_STACK_UNDERFLOW) == ERR_STACK_UNDERFLOW);
  freeVMContext();

  // call 실패 테스트
  // PC가 DS보다 뒤에 있음
  ctx = setupContext();
  ctx->cs = 0;
  ctx->ds = 128;
  call(512);
  assert((ctx->flags & ERR_INVALID_PC) == ERR_INVALID_PC);
  freeVMContext();

  // ret 실패 테스트
  // 복귀할 PC가 DS보다 뒤에 있음
  ctx = setupContext();
  ctx->memory[ctx->bp - 1] = ctx->ds + 10;
  ctx->memory[ctx->bp] = ctx->ss + 10;
  ret(0);
  assert((ctx->flags & ERR_INVALID_PC) == ERR_INVALID_PC);
  freeVMContext();

  // ret 실패 테스트
  // 복귀할 BP는 Stack 영역 내부지만 그 위에 return address를 넣을 공간이 없음
  ctx = setupContext();
  ctx->memory[ctx->bp - 1] = ctx->cs + 10;
  ctx->memory[ctx->bp] = ctx->ss + 1;
  ret(0);
  assert((ctx->flags & ERR_INVALID_BP) == ERR_INVALID_BP);
  freeVMContext();

  printf("test : function instructions pass\n");
}

void testBranchInstructions(void) {
  // ujp 테스트
  VMContext *ctx = setupContext();
  ujp(1024);
  assert(ctx->pc == 1024);
  freeVMContext();

  // tjp 테스트
  ctx = setupContext();
  pushCPUStack(1);
  tjp(1536);
  assert(ctx->pc == 1536);
  freeVMContext();

  // tjp 테스트
  // 분기 안함
  ctx = setupContext();
  int preserved_pc = ctx->pc;
  pushCPUStack(0);
  tjp(1536);
  assert(ctx->pc == preserved_pc);
  freeVMContext();

  // fjp 테스트
  ctx = setupContext();
  pushCPUStack(0);
  fjp(1400);
  assert(ctx->pc == 1400);
  freeVMContext();

  // fjp 테스트
  // 분기 안함
  ctx = setupContext();
  preserved_pc = ctx->pc;
  pushCPUStack(1);
  fjp(1400);
  assert(ctx->pc == preserved_pc);
  freeVMContext();

  // ujp 실패
  // pc가 ds보다 뒤
  ctx = setupContext();
  ctx->cs = 0;
  ctx->ds = 128;
  ujp(512);
  assert((ctx->flags & ERR_INVALID_PC) == ERR_INVALID_PC);
  freeVMContext();

  // tjp 실패
  // pc가 ds보다 뒤
  ctx = setupContext();
  ctx->cs = 0;
  ctx->ds = 128;
  pushCPUStack(1);
  tjp(512);
  assert((ctx->flags & ERR_INVALID_PC) == ERR_INVALID_PC);
  freeVMContext();

  // fjp 실패
  // pc가 ds보다 뒤
  ctx = setupContext();
  ctx->cs = 0;
  ctx->ds = 128;
  pushCPUStack(0);
  fjp(512);
  assert((ctx->flags & ERR_INVALID_PC) == ERR_INVALID_PC);
  freeVMContext();

  printf("test : branch instructions pass\n");
}

void testDataMovementInstructions(void) {
  // lod 테스트
  VMContext *ctx = setupContext();
  int addr = ctx->ds + 16;
  setSymbol(0, GLOBAL_BLOCK, addr);
  ctx->memory[addr] = 55;
  lod(0);
  assert(popCPUStack() == 55);
  freeVMContext();

  // lda 테스트
  ctx = setupContext();
  addr = ctx->ds + 32;
  setSymbol(0, GLOBAL_BLOCK, addr);
  lda(0);
  assert(popCPUStack() == addr);
  freeVMContext();

  // ldc 테스트
  ctx = setupContext();
  ldc(999);
  assert(popCPUStack() == 999);
  freeVMContext();

  // str 테스트
  ctx = setupContext();
  addr = ctx->ds + 48;
  setSymbol(0, GLOBAL_BLOCK, addr);
  pushCPUStack(777);
  str(0);
  assert(ctx->memory[addr] == 777);
  freeVMContext();

  // ldi 테스트
  ctx = setupContext();
  addr = ctx->ds + 64;
  ctx->memory[addr] = 888;
  pushCPUStack(addr);
  ldi(0);
  assert(popCPUStack() == 888);
  freeVMContext();

  // sti 테스트
  ctx = setupContext();
  addr = ctx->ds + 80;
  pushCPUStack(333);
  pushCPUStack(addr);
  sti(0);
  assert(ctx->memory[addr] == 333);
  freeVMContext();

  // lod 실패
  // 주소가 ds보다 앞임
  ctx = setupContext();
  setSymbol(0, GLOBAL_BLOCK, ctx->ds - 4);
  lod(0);
  assert((ctx->flags & ERR_INVALID_ADDR) == ERR_INVALID_ADDR);
  assert(ctx->cpu_stack.top == 0);
  freeVMContext();

  // str 실패
  // 주소가 bp임
  ctx = setupContext();
  setSymbol(0, GLOBAL_BLOCK + 1, ctx->bp);
  pushCPUStack(1234);
  str(0);
  assert((ctx->flags & ERR_INVALID_ADDR) == ERR_INVALID_ADDR);
  freeVMContext();

  // ldi 실패
  // 주소가 ds보다 앞임
  ctx = setupContext();
  pushCPUStack(ctx->ds - 2);
  ldi(0);
  assert((ctx->flags & ERR_INVALID_ADDR) == ERR_INVALID_ADDR);
  freeVMContext();

  // sti 실패
  //  주소가 ds보다 앞
  ctx = setupContext();
  pushCPUStack(111);
  pushCPUStack(ctx->ds - 2);
  sti(0);
  assert((ctx->flags & ERR_INVALID_ADDR) == ERR_INVALID_ADDR);
  freeVMContext();

  printf("test : data movement instructions pass\n");
}

void testArithmeticInstructions(void) {
  // 산술 연산 명령어 테스트
  assert(runBinaryInst(add, 7, 5) == 12);
  assert(runBinaryInst(sub, 7, 5) == 2);
  assert(runBinaryInst(mult, -3, 6) == -18);
  assert(runBinaryInst(divI, 20, 4) == 5);
  assert(runBinaryInst(mod, 20, 6) == 2);
  assert(runUnaryInst(neg, 10) == -10);

  printf("test : arithmetic instructions pass\n");
}

void testComparisonInstructions(void) {
  // 이항 연산 명령어 테스트
  assert(runBinaryInst(gt, 9, 4) == 1);
  assert(runBinaryInst(gt, 4, 9) == 0);
  assert(runBinaryInst(lt, 2, 5) == 1);
  assert(runBinaryInst(ge, 5, 5) == 1);
  assert(runBinaryInst(ge, 4, 5) == 0);
  assert(runBinaryInst(le, 4, 5) == 1);
  assert(runBinaryInst(le, 6, 5) == 0);
  assert(runBinaryInst(eq, 3, 3) == 1);
  assert(runBinaryInst(eq, 3, 4) == 0);
  assert(runBinaryInst(ne, 3, 4) == 1);
  assert(runBinaryInst(ne, 4, 4) == 0);

  printf("test : comparison instructions pass\n");
}

void testLogicalInstructions(void) {
  // 논리 연산 명령어 테스트
  assert(runBinaryInst(andI, 1, 0) == 0);
  assert(runBinaryInst(andI, 1, 1) == 1);
  assert(runBinaryInst(orI, 0, 0) == 0);
  assert(runBinaryInst(orI, 0, 3) == 1);
  assert(runUnaryInst(not, 0) == 1);
  assert(runUnaryInst(not, 5) == 0);

  printf("test : logical instructions pass\n");
}

void testDecodeArgSignExtension(void) {
  int inst = (-2) & 0x03FFFFFF;
  assert(decodeArg(inst) == -2);

  inst = 123456 & 0x03FFFFFF;
  assert(decodeArg(inst) == 123456);

  printf("test : decodeArg sign extension pass\n");
}
