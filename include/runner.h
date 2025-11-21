#pragma once

#include "core/constants.h"

#define INIT_OUTPUT_BUF_CAPACITY 50

typedef struct {
  char *out_buf;
  int buf_capacity;
  int buf_len;
} Output;

typedef struct {
  int memory[INIT_MEMORY_SIZE];
  int cpu_stack[INIT_CPU_STACK_CAPACITY];
  int cpu_top;
  int cs;
  int pc;
  int ds;
  int ss;
  int sp;
  int bp;
  int flags;
} Snapshot;

typedef struct {
  Snapshot *snapshot_list;
  int snapshot_count;
  int snapshot_capacity;
} SnapshotList;

SnapshotList *getSnapshotList();

void saveChanges();
void initSnapshot();
void initSnapshotList();
void freeSnapshotList();
void expandSnapshotList();
void makeSnapshot();
void readyToRun();
void step();
int runner();
