#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int OpenFile(const char *pathname, int flags) {
  int fd = open(pathname, flags);
  if (fd < 0) {
    perror("open");
    exit(1);
  }
  return fd;
}

ssize_t ReadFile(int fd, void *buf, size_t count) {
  ssize_t n = read(fd, buf, count);
  if (n < 0) {
    perror("read");
    exit(1);
  }
  return n;
}

ssize_t WriteFile(int fd, const void *buf, size_t count) {
  ssize_t n = write(fd, buf, count);
  if (n < 0) {
    perror("write");
    exit(1);
  }
  return n;
}

void CloseFile(int fd) {
  if (close(fd) < 0) {
    perror("close");
    exit(1);
  }
}