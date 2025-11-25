#include "assembler/assemble_error.h"

#include <stdio.h>

static const ErrorMsg assemble_error_table[] = {
    {ASSEMBLE_ERR_INVALID_FORMAT, "unknown command"},
    {ASSEMBLE_ERR_VAR_DUP, "duplicate symbol"},
    {ASSEMBLE_ERR_VAR_UNDEF, "symbol not found"},
    {ASSEMBLE_ERR_LABEL_DUP, "duplicate label"},
    {ASSEMBLE_ERR_LABEL_UNDEF, "label not found"},
    {ASSEMBLE_ERR_ARG_COUNT, "wrong operand count"},
    {ASSEMBLE_ERR_ARG_TYPE, "invalid operand type"},
    {ASSEMBLE_ERR_MEMORY, "memory allocation failed"},
    {ASSEMBLE_ERR_RETURN, "ret inst not found"},
    {ASSEMBLE_ERR_PROC, "proc inst not found"},
};

static const int assemble_error_table_count =
    sizeof(assemble_error_table) / sizeof(assemble_error_table[0]);

AssembleError returnError(AssembleError code, int line) {
  const char *msg = "unknown error";

  for (int i = 0; i < assemble_error_table_count; i++) {
    if (assemble_error_table[i].code == code) {
      msg = assemble_error_table[i].msg;
      break;
    }
  }

  printf("[ERROR] ucode Line %d: %s\n", line, msg);
  return code;
}