#include "include.h"

/*
PowerShell:
gcc --% -IC:/msys64/mingw64/bin/../include/gtk-4.0 -IC:/msys64/mingw64/bin/../include/pango-1.0 -IC:/msys64/mingw64/bin/../include/gdk-pixbuf-2.0 -IC:/msys64/mingw64/bin/../include/cairo -IC:/msys64/mingw64/bin/../include/graphene-1.0 -IC:/msys64/mingw64/bin/../lib/graphene-1.0/include -mfpmath=sse -msse -msse2 -IC:/msys64/mingw64/bin/../include -IC:/msys64/mingw64/bin/../include/harfbuzz -IC:/msys64/mingw64/bin/../include/freetype2 -IC:/msys64/mingw64/bin/../include/glib-2.0 -IC:/msys64/mingw64/bin/../lib/glib-2.0/include -IC:/msys64/mingw64/bin/../include/webp -DLIBDEFLATE_DLL -IC:/msys64/mingw64/bin/../include/pixman-1 -IC:/msys64/mingw64/bin/../include/libpng16 -IC:/msys64/mingw64/bin/../include/fribidi .\test.c -o .\test.exe -LC:/msys64/mingw64/bin/../lib -lgtk-4 -lpangowin32-1.0 -lharfbuzz -lpangocairo-1.0 -lpango-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lcairo -lgraphene-1.0 -lgio-2.0 -lglib-2.0 -lintl -lgobject-2.0
*/
void printUserdata(GtkWidget *widget, gpointer data) {
    printf("Password: %s\n", gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(data)));
}

static void start(GtkApplication *app) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new (app);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Create a new button
    GtkEntryBuffer *usernameBuffer = gtk_entry_buffer_new(NULL, 0);
    GtkEntryBuffer *passwordBuffer = gtk_password_entry_buffer_new();
    GtkWidget *usernameEntry = gtk_entry_new();
    g_object_set(usernameEntry, "placeholder-text", "Username", NULL);
    GtkWidget *passwordEntry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY (passwordEntry), true);
    g_object_set(passwordEntry, "placeholder-text", "Password", NULL);
    GtkWidget *loginButton = gtk_button_new_with_label("Login");
    gtk_window_set_title (GTK_WINDOW (window), "OpenQwystal");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    gtk_window_set_child (GTK_WINDOW (window), box);
    gtk_box_append(GTK_BOX(box), usernameEntry);
    gtk_box_append(GTK_BOX(box), passwordEntry);
    gtk_box_append(GTK_BOX(box), loginButton);
    gtk_entry_set_buffer(GTK_ENTRY(usernameEntry), usernameBuffer);
    gtk_entry_set_buffer(GTK_ENTRY(passwordEntry), passwordBuffer);
    printf("Username: %s\n", gtk_entry_buffer_get_text(usernameBuffer));
    printf("Password: %s\n", gtk_entry_buffer_get_text(passwordBuffer));
    g_signal_connect_swapped (loginButton, "clicked", G_CALLBACK (printUserdata), usernameBuffer);
    //g_signal_connect_swapped (loginButton, "clicked", G_CALLBACK (gtk_window_close), window);
    gtk_window_present (GTK_WINDOW (window));

    // Put everything here
}

void initializeWindow() {
    GtkApplication *app = gtk_application_new("com.qwystal.OpenQwystal", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(start), NULL);
    g_application_run (G_APPLICATION (app), 0, NULL);
    start(app);
}