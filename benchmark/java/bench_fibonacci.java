public final class bench_fibonacci {
  private bench_fibonacci() {}

  private static int fib(int n) {
    if (n <= 1)
      return n;
    return fib(n - 1) + fib(n - 2);
  }

  public static void main(String[] args) {
    int n = 30;
    fib(n);
  }
}
