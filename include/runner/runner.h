#pragma once

#include "core/constants.h"
#include "core/types.h"

// 명령어 그룹 초기화
void initInstGroup();
void step(void);
char *formatRunError(int line);
ErrorResult runner();
ErrorResult fastRunner();

void execInstGroup0(int inst);
void execInstGroup1(int inst);
void execInstGroup2(int inst);
void execInstGroup3(int inst);
void execInstGroup4(int inst);
