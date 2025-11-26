#pragma once

#include "core/types.h"

void catString(const char *s);
void expandOutBuffer();

void saveChanges();
void initSnapshot();
void expandSnapshotList();
void makeSnapshot();
