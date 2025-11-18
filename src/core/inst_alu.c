#include "core/inst.h"
#include "core/stack_operations.h"
#include "core/vm_context.h"
#include <stdio.h>

void add(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b + a);
}

void sub(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b - a);
}

void mult(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(a * b);
}
void divI(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b / a);
}
void mod(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b % a);
}
void gt(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b > a);
}
void lt(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b < a);
}
void ge(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b >= a);
}

void le(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b <= a);
}

void eq(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b == a);
}

void ne(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b != a);
}

void andI(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b && a);
}

void orI(int arg) {
  VMContext *ctx = getVMContext();
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b || a);
}

void not(int arg) {
  VMContext *ctx = getVMContext();
  int item = popCPUStack();
  pushCPUStack(!item);
}

void neg(int arg) {
  VMContext *ctx = getVMContext();
  int item = popCPUStack();
  pushCPUStack(-1 * item);
}