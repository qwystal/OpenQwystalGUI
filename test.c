#include <gtk/gtk.h>

/*
PowerShell:
gcc --% -IC:/msys64/mingw64/bin/../include/gtk-4.0 -IC:/msys64/mingw64/bin/../include/pango-1.0 -IC:/msys64/mingw64/bin/../include/gdk-pixbuf-2.0 -IC:/msys64/mingw64/bin/../include/cairo -IC:/msys64/mingw64/bin/../include/graphene-1.0 -IC:/msys64/mingw64/bin/../lib/graphene-1.0/include -mfpmath=sse -msse -msse2 -IC:/msys64/mingw64/bin/../include -IC:/msys64/mingw64/bin/../include/harfbuzz -IC:/msys64/mingw64/bin/../include/freetype2 -IC:/msys64/mingw64/bin/../include/glib-2.0 -IC:/msys64/mingw64/bin/../lib/glib-2.0/include -IC:/msys64/mingw64/bin/../include/webp -DLIBDEFLATE_DLL -IC:/msys64/mingw64/bin/../include/pixman-1 -IC:/msys64/mingw64/bin/../include/libpng16 -IC:/msys64/mingw64/bin/../include/fribidi .\test.c -o .\test.exe -LC:/msys64/mingw64/bin/../lib -lgtk-4 -lpangowin32-1.0 -lharfbuzz -lpangocairo-1.0 -lpango-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lcairo -lgraphene-1.0 -lgio-2.0 -lglib-2.0 -lintl -lgobject-2.0
*/

static void on_activate (GtkApplication *app) {
  // Create a new window
  GtkWidget *window = gtk_application_window_new (app);
  // Create a new button
  GtkWidget *button = gtk_button_new_with_label ("Hello, World!");
  // When the button is clicked, close the window passed as an argument
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_close), window);
  gtk_window_set_child (GTK_WINDOW (window), button);
  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char *argv[]) {
  // Create a new application
  GtkApplication *app = gtk_application_new ("com.example.GtkApplication",
                                             G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
  return g_application_run (G_APPLICATION (app), argc, argv);
}