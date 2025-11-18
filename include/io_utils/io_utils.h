#pragma once

int loadUco(const char *path, char ***lines, int *line_count);
int saveUco(const char *path, const char *content);
void freeUco(char **lines, int line_count);