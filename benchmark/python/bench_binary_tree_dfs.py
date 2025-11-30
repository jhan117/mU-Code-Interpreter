def dfs(tree: list[int], size: int, index: int) -> None:
  if index > size:
    return
  dfs(tree, size, index * 2)
  dfs(tree, size, index * 2 + 1)


def main() -> None:
  size = 10001
  tree = [0] * (size + 1)
  i = 1
  while i <= size:
    tree[i] = i
    i += 1
  dfs(tree, size, 1)


if __name__ == "__main__":
  main()
