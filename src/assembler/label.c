#include "assemble.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const Label system_labels[] = {{"read", -1}, {"write", -2}, {"lf", -3}};
static const int system_label_count =
    sizeof(system_labels) / sizeof(system_labels[0]);

int isSystemLabel(const char *name) {
  for (int i = 0; i < system_label_count; i++) {
    if (strcmp(system_labels[i].name, name) == 0)
      return 1;
  }
  return 0;
}

int findLabel(const char *name) {
  LabelList *t = &getVMContext()->labels;

  for (int i = 0; i < t->count; i++) {
    if (strcmp(t->labels[i].name, name) == 0)
      return t->labels[i].addr;
  }
  for (int i = 0; i < system_label_count; i++) {
    if (strcmp(system_labels[i].name, name) == 0)
      return system_labels[i].addr;
  }
  return -1; // 못 찾음
}

void addLabel(const char *name, int addr) {
  LabelList *t = &getVMContext()->labels;

  if (t->count >= t->capacity) {
    int newcap = t->capacity == 0 ? 8 : t->capacity * 2;
    Label *p = realloc(t->labels, sizeof(Label) * newcap);
    if (!p)
      return;
    t->labels = p;
    t->capacity = newcap;
  }

  strncpy(t->labels[t->count].name, name, MAX_LABEL_NAME_LEN - 1);
  t->labels[t->count].name[MAX_LABEL_NAME_LEN - 1] = '\0';
  t->labels[t->count].addr = addr;
  t->count++;
}

void addPatch(int addr, const char *name) {
  PatchList *t = &getVMContext()->patches;

  if (t->count >= t->capacity) {
    int newcap = t->capacity == 0 ? 8 : t->capacity * 2;
    Patch *p = realloc(t->patches, sizeof(Patch) * newcap);
    t->patches = p;
    t->capacity = newcap;
  }
  t->patches[t->count].core_idx = addr;
  strncpy(t->patches[t->count].label_name, name, MAX_LABEL_NAME_LEN - 1);
  t->patches[t->count].label_name[MAX_LABEL_NAME_LEN - 1] = '\0';
  t->count++;
}

int applyPatches() {
  PatchList *pt = &getVMContext()->patches;
  int *memory = getVMContext()->memory;

  for (int i = 0; i < pt->count; i++) {
    Patch *p = &pt->patches[i];
    int la = findLabel(p->label_name);
    if (la == -1) {
      printf("[ERROR] Patch %d: label '%s' missing\n", i, p->label_name);
      return ASSEMBLE_ERR_LABEL_MISSING;
    }
    int old = memory[p->core_idx];
    int opcode = (old >> 26) & 0x3F;
    memory[p->core_idx] = (opcode << 26) | (la & 0x03FFFFFF);
  }
  return ASSEMBLE_OK;
}
