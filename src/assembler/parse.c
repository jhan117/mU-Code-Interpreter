#include "assembler/assemble_utils.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static char *trim(char *s) {
  while (isspace(*s))
    s++;
  char *end = s + strlen(s) - 1;
  while (end >= s && isspace(*end))
    end--;
  *(end + 1) = '\0';
  return s;
}

void strcopy(char *dst, const char *src, int size) {
  if (size == 0)
    return;

  memmove(dst, src, size - 1);
  dst[size - 1] = '\0';
}

AssembleError parseLine(const char *line, char *label, char *opcode,
                        char *operands[4], int *operand_count) {
  char buf[LINE_BUFFER_LEN];
  strcopy(buf, line, LINE_BUFFER_LEN);

  // 주석 무시
  char *comment = strchr(buf, '%');
  if (comment)
    *comment = '\0';

  int line_len = strlen(buf);

  // 11열은 반드시 공백
  if (line_len < MAX_LABEL_LEN || buf[MAX_LABEL_LEN - 1] != ' ')
    return ASSEMBLE_ERR_INVALID_FORMAT;
  // 12열 이후 뭐라도 있는지 확인
  char *ptr = buf + MAX_LABEL_LEN;
  while (isspace(*ptr))
    ptr++;
  if (*ptr == '\0')
    return ASSEMBLE_ERR_INVALID_FORMAT;

  // 1~10열: 라벨
  strcopy(label, buf, MAX_LABEL_LEN);
  strcopy(label, trim(label), MAX_LABEL_LEN);

  // 첫 토큰: opcode
  char *tok = strtok(ptr, " \t\r\n");
  if (!tok)
    return ASSEMBLE_ERR_INVALID_FORMAT;
  strcpy(opcode, tok);

  *operand_count = 0;
  // 나머지 토큰: operands
  while ((tok = strtok(NULL, " \t\r\n")) != NULL) {
    if (*operand_count >= MAX_OPERANDS)
      return ASSEMBLE_ERR_ARG_COUNT;

    operands[*operand_count] = malloc(strlen(tok) + 1);
    if (!operands[*operand_count])
      return ASSEMBLE_ERR_MEMORY;

    strcpy(operands[*operand_count], tok);

    (*operand_count)++;
  }

  return ASSEMBLE_ERR_NONE;
}
