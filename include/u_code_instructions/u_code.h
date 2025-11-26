#pragma once

#include "core/vm_context.h"

int checkError(VMContext *ctx, const int *addr, const int *pc, const int *bp,
               const int *sp);

int pushCPUStack(int);
int popCPUStack(void);

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
void dup(int arg);

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

int reqWrite(const char *s);
int reqRead();
void reqLf();
