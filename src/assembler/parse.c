#include "assemble.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static char *ltrim(char *s) {
  while (isspace(*s))
    s++;
  return s;
}

static char *rtrim(char *s) {
  char *back = s + strlen(s);
  while (isspace(*--back))
    ;
  *(back + 1) = '\0';
  return s;
}

static char *trim(char *s) { return rtrim(ltrim(s)); }

int parseLine(const char *line, char *label, char *opcode, char *operands[4],
              int *operand_count) {
  *operand_count = 0;

  char buf[256];
  strncpy(buf, line, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  // 주석 제거
  char *comment = strchr(buf, '%');
  if (comment)
    *comment = '\0';

  size_t len = strlen(buf);

  // 1~10열: 라벨
  if (len >= 10) {
    strncpy(label, buf, 10);
    label[10] = '\0';
    strcpy(label, trim(label));
  } else {
    label[0] = '\0';
  }

  // 11열은 반드시 공백
  if (len < 11 || buf[10] != ' ')
    return ASSEMBLE_ERR_INVALID_FORMAT;

  // 12열 이후 첫 글자로 넘어감
  char *ptr = buf + 11;
  while (*ptr == ' ' || *ptr == '\t')
    ptr++;
  if (*ptr == '\0')
    return ASSEMBLE_ERR_INVALID_FORMAT;

  // 첫 토큰: opcode
  char *tok = strtok(ptr, " \t");
  if (!tok)
    return ASSEMBLE_ERR_INVALID_FORMAT;
  strncpy(opcode, trim(tok), 4);
  opcode[4] = '\0';

  // 나머지 토큰: operands
  while ((tok = strtok(NULL, " \t")) != NULL) {
    if (*operand_count >= 4)
      return ASSEMBLE_ERR_ARG_COUNT;
    operands[*operand_count] = malloc(strlen(tok) + 1);
    if (!operands[*operand_count])
      return ASSEMBLE_ERR_MEMORY;
    strcpy(operands[*operand_count], trim(tok));
    (*operand_count)++;
  }

  return ASSEMBLE_OK;
}