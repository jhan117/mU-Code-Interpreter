#pragma once

#include "core/constants.h"
#include "core/types.h"

#define INIT_OUTPUT_BUF_CAPACITY 50

typedef struct {
  char *out_buf;
  int buf_capacity;
  int buf_len;
} Output;

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
