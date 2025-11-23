#pragma once

#include "core/vm_context.h"
#include "runner.h"

#define SNAPSHOT_COUNT 2
#define CHANGES_PER_SNAPSHOT 9

extern ChangeList dummy_changes;
void initDummyVM();