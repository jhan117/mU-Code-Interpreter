#pragma once

#define INIT_OUTPUT_BUF_CAPACITY 50

typedef struct {
  char *out_buf;
  int buf_capacity;
  int buf_len;
} Output;

int decodeGroup(int inst);
int decodeOpcode(int inst);
int decodeArg(int inst);