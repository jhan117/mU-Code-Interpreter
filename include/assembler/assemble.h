#pragma once

#include "assembler/assemble_error.h"
#include "core/types.h"

ErrorResult assemble(char **lines, int line_count);
char *printAssembleRes();
