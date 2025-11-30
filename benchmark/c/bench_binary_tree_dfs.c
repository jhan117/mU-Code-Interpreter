static void dfs(const int tree[], int size, int index) {
  if (index > size)
    return;

  dfs(tree, size, index * 2);
  dfs(tree, size, index * 2 + 1);
}

int main(void) {
  int tree[10002];
  int size = 10001;
  int i = 1;

  while (i <= size) {
    tree[i] = i;
    i++;
  }

  dfs(tree, size, 1);
  return 0;
}
