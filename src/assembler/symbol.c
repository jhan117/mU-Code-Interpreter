#include "assemble.h"

#include <stdlib.h>

int findSymbol(int block, int offset) {
  SymbolList *t = &getVMContext()->symbols;

  for (int i = 0; i < t->count; i++) {
    if (t->symbols[i].block == block && t->symbols[i].offset == offset)
      return t->symbols[i].index;
  }
  return -1;
}

void addSymbol(int block, int offset, int size) {
  SymbolList *t = &getVMContext()->symbols;

  if (findSymbol(block, offset) != -1)
    return;

  if (t->count >= t->capacity) {
    t->capacity = t->capacity ? t->capacity * 2 : 16;
    t->symbols = realloc(t->symbols, sizeof(Symbol) * t->capacity);
  }
  t->symbols[t->count].block = block;
  t->symbols[t->count].offset = offset;
  t->symbols[t->count].size = size;
  t->symbols[t->count].index = t->count;
  t->count++;
}