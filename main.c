#include <gtk/gtk.h>

static void on_new_game_clicked(GtkWidget *button, gpointer data) {
    g_print("Starting New Game...\n");
}

static void on_settings_clicked(GtkWidget *button, gpointer data) {
    g_print("Opening Settings...\n");
}

static void on_how_to_play_clicked(GtkWidget *button, gpointer data) {
    g_print("Showing How to Play...\n");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *new_game_button;
    GtkWidget *settings_button;
    GtkWidget *how_to_play_button;

    // Create a new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CS355 Snake Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    // Apply CSS styling
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "style.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(css_provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Create a vertical box layout with padding and set alignment
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(box, 30);
    gtk_widget_set_margin_bottom(box, 30);
    gtk_widget_set_margin_start(box, 30);
    gtk_widget_set_margin_end(box, 30);

    // Create a title label with a CSS class
    title = gtk_label_new("CS355 Snake Game");
    gtk_widget_add_css_class(GTK_WIDGET(title), "title"); // Add title class for styling
    gtk_box_append(GTK_BOX(box), title);

    // Create "New Game" button and set its size
    new_game_button = gtk_button_new_with_label("New Game");
    gtk_widget_set_size_request(new_game_button, 200, -1); // Set width to 200 pixels
    g_signal_connect(new_game_button, "clicked", G_CALLBACK(on_new_game_clicked), NULL);
    gtk_box_append(GTK_BOX(box), new_game_button);

    // Create "Settings" button and set its size
    settings_button = gtk_button_new_with_label("Settings");
    gtk_widget_set_size_request(settings_button, 200, -1); // Set width to 200 pixels
    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_settings_clicked), NULL);
    gtk_box_append(GTK_BOX(box), settings_button);

    // Create "How To Play" button and set its size
    how_to_play_button = gtk_button_new_with_label("How To Play");
    gtk_widget_set_size_request(how_to_play_button, 200, -1); // Set width to 200 pixels
    g_signal_connect(how_to_play_button, "clicked", G_CALLBACK(on_how_to_play_clicked), NULL);
    gtk_box_append(GTK_BOX(box), how_to_play_button);

    // Add the box layout to the window
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Show the window
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
