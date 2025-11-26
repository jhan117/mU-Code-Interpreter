#pragma once

#include "assembler/assemble_error.h"

AssembleError assemble(char **lines, int line_count);
char *printAssembleRes();