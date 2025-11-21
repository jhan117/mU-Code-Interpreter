#pragma once

typedef struct {
  char **ucode_lines;
  int *opcode;
  int line_count;
} UCodeLines;

int loadUco(const char *path, char ***lines, int *line_count);
int saveUco(const char *path, const char *content);
void freeUco(char **lines, int line_count);

int saveLst(const char *path, UCodeLines *lines);