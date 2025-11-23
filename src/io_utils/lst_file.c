#include "core/inst.h"
#include "core/opcode.h"
#include "core/opcode_utils.h"
#include "core/vm_context.h"
#include "io_utils/io_utils.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int saveLst(const char *path, UCodeLines *lines) {
  int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0)
    return 0;

  int nbytes;
  char buf[128];
  VMContext *ctx = getVMContext();
  OutputBuffer *output = getOutputBuffer();
  const OpcodeData *opcode_data = getOpcodeData();

  const char *src_header = "======= 원본 코드 =======";
  nbytes = snprintf(buf, sizeof(buf), "%-35s %-12s %-10s %7s\n", src_header,
                    "Encoded", "Opcode", "Operand");
  write(fd, buf, nbytes);

  for (int i = 0; i < lines->line_count; i++) {
    int inst = lines->opcode[i];
    int opcode_value = getOpcodeFromInst(inst);
    int operand = decodeArg(inst);
    nbytes = snprintf(buf, sizeof(buf), "%-27s 0x%08X %8d %10d\n",
                      lines->ucode_lines[i], inst, opcode_value, operand);
    write(fd, buf, nbytes);
  }

  // 실행 결과
  nbytes = snprintf(buf, sizeof(buf), "===========실행 결과=========\n");
  write(fd, buf, nbytes);
  write(fd, output->data, output->length);

  // 명령어 사용 횟수
  nbytes = snprintf(buf, sizeof(buf), "\n========명령어 사용 횟수======\n");
  write(fd, buf, nbytes);
  for (int i = 0; i < opcode_data->count; i++) {
    // TODO: 명령어 사용 횟수 넣기
  }

  // 명령어 실행 횟수
  nbytes = snprintf(buf, sizeof(buf), "\n======명령어 실행 횟수======\n");
  write(fd, buf, nbytes);
  for (int i = 0; i < opcode_data->count; i++) {
    int opcode_value = opcode_data->codes[i];
    const char *opcode_name = opcode_data->names[opcode_value];
    if (!opcode_name)
      opcode_name = "";
    nbytes = snprintf(buf, sizeof(buf), "%-5s = %3d    ", opcode_name,
                      ctx->stat.inst_run_count[opcode_value]);
    write(fd, buf, nbytes);
    if ((i + 1) % 3 == 0)
      write(fd, "\n", sizeof(char));
    else
      write(fd, "  ", sizeof(char));
  }

  close(fd);
  freeOutBuffer();

  return 1;
}

int loadLst(const char *path, char ***lines, int *line_count) {
  int fd = open(path, O_RDONLY);
  if (fd < 0)
    return 0;

  int capacity = 100;
  *lines = malloc(sizeof(char *) * capacity);
  if (!*lines) {
    close(fd);
    return 0;
  }
  *line_count = 0;

  char buffer[256];
  ssize_t n;
  int pos = 0;
  char c;
  while ((n = read(fd, &c, 1)) > 0) {
    if (c == '\n') {
      buffer[pos] = '\0';
      if (*line_count >= capacity) {
        capacity *= 2;
        char **tmp = realloc(*lines, sizeof(char *) * capacity);
        if (!tmp) {
          freeLst(*lines, *line_count);
          close(fd);
          return 0;
        }
        *lines = tmp;
      }
      (*lines)[*line_count] = malloc(pos + 1);
      memcpy((*lines)[*line_count], buffer, pos + 1);
      (*line_count)++;
      pos = 0;
    } else if (pos < sizeof(buffer) - 1) {
      buffer[pos++] = c;
    }
  }

  if (pos > 0) {
    buffer[pos] = '\0';
    if (*line_count >= capacity) {
      capacity *= 2;
      char **tmp = realloc(*lines, sizeof(char *) * capacity);
      if (!tmp) {
        freeLst(*lines, *line_count);
        close(fd);
        return 0;
      }
      *lines = tmp;
    }
    (*lines)[*line_count] = malloc(pos + 1);
    memcpy((*lines)[*line_count], buffer, pos + 1);
    (*line_count)++;
  }

  close(fd);
  return 1;
}

void freeLst(char **lines, int line_count) {
  if (!lines)
    return;

  for (int i = 0; i < line_count; i++) {
    free(lines[i]);
  }
  free(lines);
}
