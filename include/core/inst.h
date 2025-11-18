#pragma once

#include "core/vm_context.h"

// 명령어 그룹 초기화
void initInstGroup();

// 명령어 디코딩
int decodeGroup(int inst);
int decodeOpcode(int inst);
int decodeArg(int inst);

// 명령어 오류 검사
int checkError(VMContext *ctx, int addr, int pc, int bp, int sp);

// 출력 버퍼 초기화
void initOutBuffer();

// 명령어 그룹
void execInstGroup0(int inst);
void execInstGroup1(int inst);
void execInstGroup2(int inst);
void execInstGroup3(int inst);
void execInstGroup4(int inst);

// alu 연산
void add(int arg);
void sub(int arg);
void mult(int arg);
void divI(int arg);
void mod(int arg);
void gt(int arg);
void lt(int arg);
void ge(int arg);
void le(int arg);
void eq(int arg);
void ne(int arg);
void andI(int arg);
void orI(int arg);
void not(int arg);
void neg(int arg);

// cu 연산
void proc(int arg);
void ret(int arg);
void ldp(int arg);
void push(int arg);
void call(int arg);
void ujp(int arg);
void tjp(int arg);
void fjp(int arg);
void lod(int arg);
void lda(int arg);
void ldc(int arg);
void str(int arg);
void ldi(int arg);
void sti(int arg);

void Write();
void Read();
void lf();