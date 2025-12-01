public final class bench_bubble_sort {
  private bench_bubble_sort() {}

  public static void main(String[] args) {
    final int n = 1000;
    int[] a = new int[n];
    int i = 0;

    while (i < n) {
      a[i] = n - i;
      i++;
    }

    for (i = 0; i < n - 1; i++) {
      for (int j = 0; j < n - 1 - i; j++) {
        if (a[j] > a[j + 1]) {
          int temp = a[j];
          a[j] = a[j + 1];
          a[j + 1] = temp;
        }
      }
    }
  }
}
