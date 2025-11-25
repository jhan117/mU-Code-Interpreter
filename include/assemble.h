#pragma once

#include "core/instruction.h"
#include "core/vm_context.h"
// 어셈블 에러 코드
#define ASSEMBLE_OK 0
#define ASSEMBLE_ERR_INVALID_FORMAT 1
#define ASSEMBLE_ERR_VAR_DUP 2
#define ASSEMBLE_ERR_LABEL_DUP 3
#define ASSEMBLE_ERR_LABEL_MISSING 4
#define ASSEMBLE_ERR_ARG_COUNT 5
#define ASSEMBLE_ERR_ARG_TYPE 6
#define ASSEMBLE_ERR_VAR_UNDEF 7
#define ASSEMBLE_ERR_MEMORY 8

int isSystemLabel(const char *name);
int findLabel(const char *name);
void addLabel(const char *name, int addr);
void addPatch(int addr, const char *name);
int applyPatches();
int findSymbol(int block, int offset);
void addSymbol(int block, int offset, int size);
int parseLine(const char *line, char *label, char *opcode, char *operands[4],
              int *operand_count);
int assemble(char **lines, int line_count, int *encoded);