#pragma once

int loadUco(const char *path, char ***lines, int *line_count);
void freeUco(char **lines, int line_count);
int saveUco(const char *path, const char *content);

int loadLst(const char *path, char ***lines, int *line_count);
void freeLst(char **lines, int line_count);
int saveLst(const char *path, char **lines, int line_count);
