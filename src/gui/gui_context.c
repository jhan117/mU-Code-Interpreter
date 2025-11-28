#include "gui/gui_context.h"

static GuiContext ctx;
GuiContext *getGuiContext(void) { return &ctx; }