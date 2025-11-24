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
} AssembleError;

typedef struct {
  AssembleError code;
  const char *msg;
} ErrorMsg;

AssembleError returnError(AssembleError code, int line);