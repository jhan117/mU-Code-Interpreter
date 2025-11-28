#pragma once

#include "constants.h"
#include "types.h"

typedef struct {
  int cs;
  int pc;
  int ds;
  int ss;
  int sp;
  int bp;

  int code_len;  // 코드 영역 길이
  int g_var_cnt; // 전역변수 개수
  int prev_pc;   // 현재 실행 중

  LabelList label_list;          // 라벨
  PatchList patch_list;          // 패치할 라벨
  SymbolList symbol_list;        // 변수 정보
  FuncList func_list;            // 함수 매개변수 정보
  CallPatchList call_patch_list; // 패치할 함수
  SourceMap source_map;          // 원본 소스 라인 매핑

  int run_mode;
  unsigned int flags;
  int memory[INIT_MEMORY_SIZE];                     // memory stack
  Stack cpu_stack;                                  // cpu stack
  InstructionGroup inst_group[TOTAL_OPCODE_GROUPS]; // 함수 포인터 매핑
  ChangeList changes;                               // 변경점 기록
  SnapshotList snapshot_list;                       // N단계 스냅샷 기록
  OutputBuffer output_buffer;                       // 출력
  Statistics stat;                                  // 통계
} VMContext;

VMContext *getVMContext();
void initVMContext();
void freeVMContext();
