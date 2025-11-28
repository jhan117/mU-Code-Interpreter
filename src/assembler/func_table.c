#include "assembler/assemble_utils.h"

#include <stdlib.h>
#include <string.h>

FuncInfo *addFunc(char *name, int addr) {
  FuncList *func_list = &getVMContext()->func_list;

  if (func_list->count >= func_list->capacity) {
    int new_cap = func_list->capacity * 2;
    FuncInfo *new_func = realloc(func_list->items, sizeof(FuncInfo) * new_cap);
    if (!new_func)
      return NULL;
    func_list->items = new_func;
    func_list->capacity = new_cap;
  }

  FuncInfo *func = &func_list->items[func_list->count];
  strcopy(func->name, name, MAX_LABEL_LEN);
  func->start_addr = addr;
  func->is_start = 1;
  func_list->count++;

  return func;
}

FuncInfo *findFuncByName(char *name) {
  FuncList func_list = getVMContext()->func_list;

  for (int i = 0; i < func_list.count; i++) {
    if (strcmp(func_list.items[i].name, name) == 0)
      return &func_list.items[i];
  }
  return NULL;
}

FuncInfo *findFuncProc() {
  FuncList func_list = getVMContext()->func_list;

  for (int i = 0; i < func_list.count; i++) {
    if (func_list.items[i].is_start)
      return &func_list.items[i];
  }
  return NULL;
}

AssembleError addCallPatch(const char *name, int param_cnt) {
  CallPatchList *call_patch_list = &getVMContext()->call_patch_list;

  if (call_patch_list->count >= call_patch_list->capacity) {
    int new_cap = call_patch_list->capacity * 2;
    CallPatch *new_patches =
        realloc(call_patch_list->patches, sizeof(CallPatch) * new_cap);
    if (!new_patches)
      return ASSEMBLE_ERR_MEMORY;
    call_patch_list->patches = new_patches;
    call_patch_list->capacity = new_cap;
  }

  strcopy(call_patch_list->patches[call_patch_list->count].func_name, name,
          MAX_LABEL_LEN);
  call_patch_list->patches[call_patch_list->count].param_cnt = param_cnt;
  call_patch_list->count++;

  return ASSEMBLE_ERR_NONE;
}

CallPatch *findCallPatchByName(char *name) {
  CallPatchList call_patch_list = getVMContext()->call_patch_list;

  for (int i = 0; i < call_patch_list.count; i++) {
    if (strcmp(call_patch_list.patches[i].func_name, name) == 0)
      return &call_patch_list.patches[i];
  }
  return NULL;
}

void applyCallPatch() {
  CallPatchList *call_patch_list = &getVMContext()->call_patch_list;

  for (int i = 0; i < call_patch_list->count; i++) {
    CallPatch *patch = &call_patch_list->patches[i];
    FuncInfo *func = findFuncByName(patch->func_name);
    func->param_cnt = patch->param_cnt;
  }
}