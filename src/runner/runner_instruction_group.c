#include "core/instruction.h"
#include "core/vm_context.h"
#include "runner/runner.h"
#include "runner/u_code_instructions/u_code.h"

#include <stdio.h>

void initInstGroup() {
  VMContext *ctx = getVMContext();
  ctx->inst_group[0].execInst = execInstGroup0;
  ctx->inst_group[1].execInst = execInstGroup1;
  ctx->inst_group[2].execInst = execInstGroup2;
  ctx->inst_group[3].execInst = execInstGroup3;
  ctx->inst_group[4].execInst = execInstGroup4;
}

void execInstGroup0(int inst) {
  int arg;
  int opcode;
  decodeInst(inst, NULL, &opcode, &arg);
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
  int arg;
  int opcode;
  decodeInst(inst, NULL, &opcode, &arg);
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
  int arg;
  int opcode;
  decodeInst(inst, NULL, &opcode, &arg);
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
  int arg;
  int opcode;
  decodeInst(inst, NULL, &opcode, &arg);
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
  int arg;
  int opcode;
  decodeInst(inst, NULL, &opcode, &arg);
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
  case 7:
    dup(arg);
    break;
  }
  return;
}