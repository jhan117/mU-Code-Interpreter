#pragma once

#include "core/types.h"

void catString(const char *s);
void expandOutBuffer();

void saveChanges();
void applyChanges(int start_step, int end_step);

void initSnapshot();
void expandSnapshotList();
void makeSnapshot();
void restoreSnapshot(const Snapshot *snap);
