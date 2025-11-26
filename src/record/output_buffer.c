#include "core/vm_context.h"
#include "record/record.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void catString(const char *s) {
  VMContext *ctx = getVMContext();
  int len = strlen(s);

  while (ctx->output_buffer.length + len + 1 > ctx->output_buffer.capacity) {
    expandOutBuffer();
  }

  memcpy(ctx->output_buffer.data + ctx->output_buffer.length, s, len);
  ctx->output_buffer.length += len;

  ctx->output_buffer.data[ctx->output_buffer.length] = '\0';
}

void expandOutBuffer() {
  VMContext *ctx = getVMContext();
  ctx->output_buffer.capacity *= 2;
  ctx->output_buffer.data = realloc(ctx->output_buffer.data,
                                    sizeof(char) * ctx->output_buffer.capacity);
}
