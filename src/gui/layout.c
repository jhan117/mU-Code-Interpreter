#include "gui.h"

void onActivate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "mU-Code interpreter");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start(GTK_BOX(main_box), createMenubar(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(main_box), content_box, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(content_box), createLeftPanel(), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(content_box), createRightPanel(), TRUE, TRUE, 0);

  gtk_widget_show_all(window);
}