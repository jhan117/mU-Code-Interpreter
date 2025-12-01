int main(void) {
  int a[1000];
  int n = 1000;
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
  return 0;
}
