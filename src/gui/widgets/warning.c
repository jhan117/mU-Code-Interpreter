#include "gui/gui_widgets.h"

void showMessage(GtkMessageType type, const char *message) {
  GuiContext *ctx = getGuiContext();

  GtkWidget *dialog =
      gtk_message_dialog_new(ctx->main_window, GTK_DIALOG_MODAL,
                             type, // GTK_MESSAGE_INFO, GTK_MESSAGE_WARNING,
                                   // GTK_MESSAGE_ERROR
                             GTK_BUTTONS_OK, "%s", message);

  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
