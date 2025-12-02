public final class bench_binary_tree_dfs {
  private bench_binary_tree_dfs() {}

  private static void dfs(int[] tree, int size, int index) {
    if (index > size)
      return;

    dfs(tree, size, index * 2);
    dfs(tree, size, index * 2 + 1);
  }

  public static void main(String[] args) {
    final int size = 10001;
    int[] tree = new int[size + 1];
    int i = 1;

    while (i <= size) {
      tree[i] = i;
      i++;
    }

    dfs(tree, size, 1);
  }
}
