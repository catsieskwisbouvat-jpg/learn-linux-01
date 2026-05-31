#include <gtk/gtk.h>
#include <stdlib.h>

// Deze functie controleert welke knop is ingedrukt aan de hand van 'user_data'
void on_project_clicked(GtkButton *button, gpointer user_data) {
    const gchar *les_naam = (const gchar *)user_data;
    
    // We halen de thuismap op (bijv. /home/lucas)
    const gchar *home_dir = g_get_home_dir();
    gchar *commando;

    // Controleren welke knop is ingedrukt
    if (g_strcmp0(les_naam, "basis") == 0) {
        // Bouw het exacte pad naar de executable in de map 'lessons'
        // We voegen een '&' toe zodat het hoofdmenu niet vastloopt tijdens de les
        commando = g_strdup_printf("chmod +x %s/LearnLinux/lessons/basics && %s/LearnLinux/lessons/basics &", home_dir, home_dir);
        
        g_print("De basis les wordt opgestart...\n");
        system(commando);
        g_free(commando);
    } 
    else if (g_strcmp0(les_naam, "terminal") == 0) {
        // Hier kun je later een tweede les toevoegen (bijv. ~/LearnLinux/lessons/terminal)
        g_print("Terminal les ingedrukt (nog niet gekoppeld).\n");
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *title_label;
    GtkWidget *image;
    GtkWidget *btn_les1, *btn_les2, *btn_close;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Learn Linux - Project Menu");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 480);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), "<span size='x-large' weight='bold'>Learn Linux Project</span>");
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 0);

    // Afbeelding laden uit ~/LearnLinux/mint.png
    const gchar *home_dir = g_get_home_dir(); 
    gchar *afbeelding_pad = g_build_filename(home_dir, "LearnLinux", "mint.png", NULL);
    image = gtk_image_new_from_file(afbeelding_pad);
    gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 10);
    g_free(afbeelding_pad);

    // --- DE INGEDRUKTE KNOPPEN CHECKEN ---
    // We geven als laatste argument ("basis" of "terminal") mee om te checken waar gedrukt wordt
    btn_les1 = gtk_button_new_with_label("Start Les: De Basis");
    g_signal_connect(btn_les1, "clicked", G_CALLBACK(on_project_clicked), "basis"); 

    btn_les2 = gtk_button_new_with_label("Start Les: De Terminal (Binnenkort)");
    g_signal_connect(btn_les2, "clicked", G_CALLBACK(on_project_clicked), "terminal");

    btn_close = gtk_button_new_with_label("Menu Sluiten");
    g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(gtk_widget_destroy), window);

    // Knoppen toevoegen aan de layout
    gtk_box_pack_start(GTK_BOX(vbox), btn_les1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_les2, TRUE, TRUE, 0);
    
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), btn_close, TRUE, TRUE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
