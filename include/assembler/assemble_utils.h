#pragma once

#include "assembler/assemble_error.h"
#include "core/instruction.h" // OpInfo 불러오기
#include "core/vm_context.h"

// parse.c
void strcopy(char *dst, const char *src, int size);
AssembleError parseLine(const char *line, char *label, char *opcode,
                        char *operands[4], int *operand_count);
int parseTable(const char *line, char *label, char *opcode, char *operands);

// label.c
#define LABEL_NOT_FOUND -9999
void addSystemLabel();
int isSystemLabel(char *name);
int findLabel(const char *name);
const char *findLabelByAddr(int addr);
AssembleError addLabel(const char *name, int addr);
AssembleError addPatch(int addr, int src_idx, const char *name);
AssembleError applyPatches();

// symbol.c
#define SYMBOL_NOT_FOUND -9999
int findSymbol(int block, int offset);
Symbol *findSymbolByIdx(int idx);
AssembleError addSymbol(int block, int offset, int size);
void applySymbolOffset();

// operand.c
int isNumber(const char *s);
AssembleError validOperands(const OpInfo *info, char *operands[], int count,
                            int *operand_val, int addr, int src_idx);

// func_table.c
FuncInfo *addFunc(char *name, int addr);
FuncInfo *findFuncByName(char *name);
FuncInfo *findFuncProc();
AssembleError addCallPatch(const char *name, int param_cnt);
CallPatch *findCallPatchByName(char *name);
AssembleError applyCallPatch();
