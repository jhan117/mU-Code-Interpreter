static int fib(int n) {
  if (n <= 1)
    return n;
  return fib(n - 1) + fib(n - 2);
}

int main(void) {
  int n = 30;
  int result = fib(n);
  (void)result;
  return 0;
}
