#pragma once

#include "core/types.h"

OutputBuffer *getOutputBuffer();

// 출력 버퍼 초기화
void initOutBuffer();
void freeOutBuffer();
void catString(const char *s);
void expandOutBuffer();

void saveChanges();
void initSnapshot();
void initSnapshotList();
void freeSnapshotList();
void expandSnapshotList();
void makeSnapshot();