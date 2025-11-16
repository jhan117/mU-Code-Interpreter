#include "core/inst.h"
#include "core/vm_context.h"
#include "opcode.h"
#include "runner.h"

static InstructionGroup inst_group_map[] = {
    // 함수 정의 및 호출 (5)
    {OP_PROC, proc},
    {OP_RET, ret},
    {OP_LDP, ldp},
    {OP_PUSH, push},
    {OP_CALL, call},

    // 흐름 제어 (3)
    {OP_UJP, ujp},
    {OP_TJP, tjp},
    {OP_FJP, fjp},

    // 데이터 이동 연산자 (6)
    {OP_LOD, lod},
    {OP_LDA, lda},
    {OP_LDC, ldc},
    {OP_STR, str},
    {OP_LDI, ldi},
    {OP_STI, sti},

    // 이항 연산자 (13)
    {OP_GT, gt},
    {OP_LT, lt},
    {OP_GE, ge},
    {OP_LE, le},
    {OP_EQ, eq},
    {OP_NE, ne},
    {OP_AND, andI},
    {OP_OR, orI},

    {OP_ADD, add},
    {OP_SUB, sub},
    {OP_MULT, mult},
    {OP_DIV, divI},
    {OP_MOD, mod},

    // 단항 연산자 (2)
    {OP_NOT, not },
    {OP_NEG, neg}};

void initInstGroup() {
  VMContext *ctx = getVMContext();
  ctx->inst_group[0].execInst = execInstGroup0;
  ctx->inst_group[1].execInst = execInstGroup1;
  ctx->inst_group[2].execInst = execInstGroup2;
  ctx->inst_group[3].execInst = execInstGroup3;
  ctx->inst_group[4].execInst = execInstGroup4;
}

void execInstGroup0(int inst) {
  VMContext *ctx = getVMContext();
  int arg = decodeArg(inst);
  int opcode = decodeOpcode(inst);
  switch (opcode) {
  case 0:
    proc(arg);
    break;
  case 1:
    ret(arg);
    break;
  case 2:
    ldp(arg);
    break;
  case 3:
    push(arg);
    break;
  case 4:
    call(arg);
    break;
  }
  return;
}
void execInstGroup1(int inst) {
  VMContext *ctx = getVMContext();
  int arg = decodeArg(inst);
  int opcode = decodeOpcode(inst);
  switch (opcode) {
  case 0:
    ujp(arg);
    break;
  case 1:
    tjp(arg);
    break;
  case 2:
    fjp(arg);
    break;
  }
  return;
}

void execInstGroup2(int inst) {
  VMContext *ctx = getVMContext();
  int arg = decodeArg(inst);
  int opcode = decodeOpcode(inst);
  switch (opcode) {
  case 0:
    lod(arg);
    break;
  case 1:
    lda(arg);
    break;
  case 2:
    ldc(arg);
    break;
  case 3:
    str(arg);
    break;
  case 4:
    ldi(arg);
    break;
  case 5:
    sti(arg);
    break;
  }
  return;
}
void execInstGroup3(int inst) {
  VMContext *ctx = getVMContext();
  int arg = decodeArg(inst);
  int opcode = decodeOpcode(inst);
  switch (opcode) {
  case 0:
    gt(arg);
    break;
  case 1:
    lt(arg);
    break;
  case 2:
    ge(arg);
    break;
  case 3:
    le(arg);
    break;
  case 4:
    eq(arg);
    break;
  case 5:
    ne(arg);
    break;
  case 6:
    andI(arg);
    break;
  case 7:
    orI(arg);
    break;
  }
  return;
}
void execInstGroup4(int inst) {
  VMContext *ctx = getVMContext();
  int arg = decodeArg(inst);
  int opcode = decodeOpcode(inst);
  switch (opcode) {
  case 0:
    add(arg);
    break;
  case 1:
    sub(arg);
    break;
  case 2:
    mult(arg);
    break;
  case 3:
    divI(arg);
    break;
  case 4:
    mod(arg);
    break;
  case 5:
    not(arg);
    break;
  case 6:
    neg(arg);
    break;
  }
  return;
}