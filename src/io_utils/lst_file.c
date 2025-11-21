#include "core/vm_context.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>

// int saveLst(const char *path, UCodeLines *lines) {
//   int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC);
//   if (fd < 0) {
//     return 0;
//   }

//   // 어셈블결과, 인코딩 결과
//   int nbyte;
//   char buf[100];
//   for (int i = 0; i < lines->line_count; i++) {
//     sprintf(buf, "%s %10d\n", lines->ucode_lines[i], lines->opcode[i]);
//     nbyte = write(fd, buf, strlen(buf));
//   }
//   // 실행 결과
//   VMContext *ctx = getVMContext();
//   // 명령어 사용 횟수

//   // 명령어 실행 횟수
// }
