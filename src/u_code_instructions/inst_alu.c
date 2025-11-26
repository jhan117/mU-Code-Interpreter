#include "u_code_instructions/u_code.h"

#include <stdio.h>

void add(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b + a);
}

void sub(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b - a);
}

void mult(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(a * b);
}
void divI(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b / a);
}
void mod(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b % a);
}
void gt(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b > a);
}
void lt(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b < a);
}
void ge(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b >= a);
}

void le(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b <= a);
}

void eq(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b == a);
}

void ne(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b != a);
}

void andI(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b && a);
}

void orI(int arg) {
  (void)arg;
  int a = popCPUStack();
  int b = popCPUStack();
  pushCPUStack(b || a);
}

void not(int arg) {
  (void)arg;
  int item = popCPUStack();
  pushCPUStack(!item);
}

void neg(int arg) {
  (void)arg;
  int item = popCPUStack();
  pushCPUStack(-1 * item);
}

void dup(int arg) {
  (void)arg;
  int item = popCPUStack();
  pushCPUStack(item);
  pushCPUStack(item);
}