#pragma once

typedef enum {
  ASSEMBLE_ERR_NONE = 0,
  ASSEMBLE_ERR_INVALID_FORMAT,
  ASSEMBLE_ERR_VAR_DUP,
  ASSEMBLE_ERR_VAR_UNDEF,
  ASSEMBLE_ERR_LABEL_DUP,
  ASSEMBLE_ERR_LABEL_UNDEF,
  ASSEMBLE_ERR_ARG_COUNT,
  ASSEMBLE_ERR_ARG_TYPE,
  ASSEMBLE_ERR_MEMORY,
  ASSEMBLE_ERR_RETURN,
  ASSEMBLE_ERR_PROC,
} AssembleError;

typedef struct {
  AssembleError code;
  const char *msg;
} ErrorMsg;

typedef struct {
  AssembleError code;
  int line;
} AssembleResult;

char *formatAsmError(AssembleError code, int line);
void printAsmError(AssembleError code, int line);