#include "assembler/assemble.h"
#include "core/vm_context.h"
#include "io_utils/io_utils.h"
#include "runner/runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runAsCLI(const char *input_path, const char *output_path) {
  initVMContext();

  char **lines = NULL;
  int line_count = 0;

  if (!loadUco(input_path, &lines, &line_count)) {
    printf("[ERROR] Failed to load %s\n", input_path);
    return;
  }

  printf("====== 어셈블 시작 ======\n");
  ErrorResult res = assemble(lines, line_count);
  if (res.src != ERR_SRC_NONE) {
    printf("[ERROR] Assemble exited with error\n");
    freeUco(lines, line_count);
    return;
  }
  printf("====== 어셈블 완료 ======\n");

  printf("====== 실행 시작 ======\n");
  res = runner();
  if (res.src != ERR_SRC_NONE) {
    fprintf(stderr, "[ERROR] Runner exited with error\n");
    freeUco(lines, line_count);
    return;
  }
  printf("====== 실행 완료 ======\n");

  char lst_path[512];
  if (output_path && *output_path)
    snprintf(lst_path, sizeof(lst_path), "%s", output_path);
  else
    snprintf(lst_path, sizeof(lst_path), "%s.lst", input_path);

  if (!saveLst(lst_path, lines, line_count)) {
    printf("[ERROR] Failed to save %s\n", lst_path);
    freeUco(lines, line_count);
    return;
  } else {
    printf("%s 에 LST 파일이 생성되었습니다.\n", lst_path);
  }

  freeVMContext();
}
