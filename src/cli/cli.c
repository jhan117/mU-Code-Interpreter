#include "assembler/assemble.h"
#include "core/vm_context.h"
#include "io_utils/io_utils.h"
#include "runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeUCode(UCodeLines *ucode, int line_count) {
  if (!ucode)
    return;
  if (ucode->opcode) {
    free(ucode->opcode);
    ucode->opcode = NULL;
  }
  if (ucode->ucode_lines) {
    freeUco(ucode->ucode_lines, line_count);
    ucode->ucode_lines = NULL;
  }
}

void runAsCLI(const char *input_path, const char *output_path) {
  initVMContext();

  char **lines = NULL;
  int line_count = 0;

  if (!loadUco(input_path, &lines, &line_count)) {
    printf("[ERROR] Failed to load %s\n", input_path);
    return;
  }

  UCodeLines ucode;
  ucode.ucode_lines = lines;
  ucode.opcode = malloc(sizeof(int) * line_count);
  ucode.line_count = line_count;

  printf("====== 어셈블 시작 ======\n");
  AssembleError result = assemble(lines, line_count, ucode.opcode);
  if (result != ASSEMBLE_ERR_NONE) {
    freeUCode(&ucode, line_count);
    return;
  }
  printf("====== 어셈블 완료 ======\n");

  printf("====== 실행 시작 ======\n");
  if (runner() != 0) {
    fprintf(stderr, "[ERROR] Runner exited with error\n");
    freeUCode(&ucode, line_count);
    return;
  }
  printf("====== 실행 완료 ======\n");

  for (int i = 0; i < line_count; i++) {
    size_t len = strlen(lines[i]);
    if (len > 0 && lines[i][len - 1] == '\n') {
      lines[i][len - 1] = '\0';
    }
  }

  char lst_path[512];
  if (output_path && *output_path)
    snprintf(lst_path, sizeof(lst_path), "%s", output_path);
  else
    snprintf(lst_path, sizeof(lst_path), "%s.lst", input_path);

  if (!saveLst(lst_path, &ucode)) {
    printf("[ERROR] Failed to save %s\n", lst_path);
  } else {
    printf("%s 에 LST 파일이 생성되었습니다.\n", lst_path);
  }

  freeUCode(&ucode, line_count);
  freeVMContext();
}
