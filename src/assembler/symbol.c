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