N = 1000


def bubble_sort(values: list[int]) -> None:
  for i in range(N - 1):
    for j in range(N - 1 - i):
      if values[j] > values[j + 1]:
        temp = values[j]
        values[j] = values[j + 1]
        values[j + 1] = temp


def main() -> None:
  a = [0] * N
  i = 0
  while i < N:
    a[i] = N - i
    i += 1
  bubble_sort(a)


if __name__ == "__main__":
  main()
