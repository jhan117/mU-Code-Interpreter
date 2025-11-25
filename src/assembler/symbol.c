#include "assembler/assemble_utils.h"

#include <stdlib.h>

int findSymbol(int block, int offset) {
  SymbolList symbol_list = getVMContext()->symbol_list;
  Symbol *symbols = symbol_list.symbols;

  for (int i = 0; i < symbol_list.count; i++) {
    if (symbols[i].block == block && symbols[i].offset == offset)
      return symbols[i].index;
  }

  return SYMBOL_NOT_FOUND;
}

Symbol *findSymbolByIdx(int idx) {
  SymbolList symbol_list = getVMContext()->symbol_list;
  Symbol *symbols = symbol_list.symbols;

  for (int i = 0; i < symbol_list.count; i++) {
    if (symbols[i].index == idx)
      return &symbols[i];
  }

  return SYMBOL_NOT_FOUND;
}

AssembleError addSymbol(int block, int offset, int size) {
  SymbolList *symbol_list = &getVMContext()->symbol_list;

  if (symbol_list->count >= symbol_list->capacity) {
    int new_cap = symbol_list->capacity * 2;
    Symbol *new_symbols =
        realloc(symbol_list->symbols, sizeof(Label) * new_cap);
    if (!new_symbols)
      return ASSEMBLE_ERR_MEMORY;
    symbol_list->symbols = new_symbols;
    symbol_list->capacity = new_cap;
  }

  symbol_list->symbols[symbol_list->count].index = symbol_list->count;
  symbol_list->symbols[symbol_list->count].block = block;
  symbol_list->symbols[symbol_list->count].offset = offset;
  symbol_list->symbols[symbol_list->count].size = size;
  symbol_list->count++;

  return ASSEMBLE_ERR_NONE;
}

void applySymbolOffset() {
  VMContext *ctx = getVMContext();
  FuncList *func_list = &ctx->func_list;
  SymbolList *symbol_list = &ctx->symbol_list;
  Symbol *symbols = symbol_list->symbols;

  for (int i = 0; i < func_list->count; i++) {
    FuncInfo *func = &func_list->items[i];
    int param_cnt = func->param_cnt;

    for (int s = 0; s < symbol_list->count; s++) {
      Symbol *sym = &symbol_list->symbols[s];
      if (sym->block != func->func_block)
        continue;

      if (sym->offset < param_cnt) // 파라미터 (sym 정의 순)
        sym->offset += 1;
      else // 지역변수
        sym->offset = param_cnt - sym->offset - 2;
    }
  }

  return;
}