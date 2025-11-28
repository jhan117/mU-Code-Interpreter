#pragma once

#include <sys/types.h> // ssize_t

int loadUco(const char *path, char ***lines, int *line_count);
int saveUco(const char *path, char **lines, int line_count);
void freeUco(char **lines, int line_count);

char *makeLstString(const char **src_lines, int src_count);
int saveLst(const char *path, const char **src_lines, int src_count);

char *joinLines(char **lines, int line_count);

// wrapper.c
int OpenFile(const char *pathname, int flags);
ssize_t ReadFile(int fd, void *buf, size_t count);
ssize_t WriteFile(int fd, const void *buf, size_t count);
void CloseFile(int fd);
