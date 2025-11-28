#include "assembler/assemble_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const Label system_labels[] = {
    {"read", -1},
    {"write", -2},
    {"lf", -3},
};
static const int system_label_count =
    sizeof(system_labels) / sizeof(system_labels[0]);

void addSystemLabel() {
  LabelList *label_list = &getVMContext()->label_list;

  for (int i = 0; i < system_label_count; i++) {
    const char *name = system_labels[i].name;
    int addr = system_labels[i].addr;

    strcopy(label_list->labels[label_list->count].name, name, MAX_LABEL_LEN);
    label_list->labels[label_list->count].addr = addr;
    label_list->count++;
  }
}

int isSystemLabel(char *name) {
  for (int i = 0; i < system_label_count; i++) {
    if (strcmp(system_labels[i].name, name) == 0)
      return 1;
  }
  return 0;
}

int findLabel(const char *name) {
  LabelList label_list = getVMContext()->label_list;
  Label *labels = label_list.labels;

  for (int i = 0; i < system_label_count; i++) {
    if (strcmp(system_labels[i].name, name) == 0)
      return system_labels[i].addr;
  }

  for (int i = 0; i < label_list.count; i++) {
    if (strcmp(labels[i].name, name) == 0)
      return labels[i].addr;
  }

  return LABEL_NOT_FOUND;
}

const char *findLabelByAddr(int addr) {
  LabelList label_list = getVMContext()->label_list;
  Label *labels = label_list.labels;

  for (int i = 0; i < system_label_count; i++) {
    if (system_labels[i].addr == addr)
      return system_labels[i].name;
  }

  for (int i = 0; i < label_list.count; i++) {
    if (labels[i].addr == addr)
      return labels[i].name;
  }

  return NULL;
}

AssembleError addLabel(const char *name, int addr) {
  LabelList *label_list = &getVMContext()->label_list;

  if (label_list->count >= label_list->capacity) {
    int new_cap = label_list->capacity * 2;
    Label *new_labels = realloc(label_list->labels, sizeof(Label) * new_cap);
    if (!new_labels)
      return ASSEMBLE_ERR_MEMORY;
    label_list->labels = new_labels;
    label_list->capacity = new_cap;
  }

  strcopy(label_list->labels[label_list->count].name, name, MAX_LABEL_LEN);
  label_list->labels[label_list->count].addr = addr;
  label_list->count++;

  return ASSEMBLE_ERR_NONE;
}

AssembleError addPatch(int addr, int src_idx, const char *name) {
  PatchList *patch_list = &getVMContext()->patch_list;

  if (patch_list->count >= patch_list->capacity) {
    int new_cap = patch_list->capacity * 2;
    Patch *new_patches = realloc(patch_list->patches, sizeof(Patch) * new_cap);
    if (!new_patches)
      return ASSEMBLE_ERR_MEMORY;
    patch_list->patches = new_patches;
    patch_list->capacity = new_cap;
  }

  patch_list->patches[patch_list->count].code_idx = addr;
  patch_list->patches[patch_list->count].src_idx = src_idx;
  strcopy(patch_list->patches[patch_list->count].label_name, name,
          MAX_LABEL_LEN);
  patch_list->count++;

  return ASSEMBLE_ERR_NONE;
}

ErrorResult applyPatches() {
  ErrorResult ok = {ERR_SRC_NONE, 0, -1};

  VMContext *ctx = getVMContext();
  PatchList *patch_list = &ctx->patch_list;
  int *memory = ctx->memory;

  for (int i = 0; i < patch_list->count; i++) {
    Patch *patch = &patch_list->patches[i];
    int addr = findLabel(patch->label_name);
    if (addr == LABEL_NOT_FOUND) {
      printAsmError(ASSEMBLE_ERR_LABEL_UNDEF, patch->src_idx);
      return (ErrorResult){ERR_SRC_ASSEMBLE, ASSEMBLE_ERR_LABEL_UNDEF,
                           patch->src_idx};
    }
    memory[patch->code_idx] = patchInst(memory[patch->code_idx], addr);
  }
  return ok;
}
