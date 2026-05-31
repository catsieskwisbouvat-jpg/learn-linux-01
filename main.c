#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    GtkWidget *dialog;
    GtkWidget *progress_bar;
    GtkWidget *label;
    GtkWidget *install_button;
} InstallerData;

void on_install_clicked(GtkButton *button, gpointer user_data);

// Handige functie om het .desktop bestand te schrijven
void maak_bureaublad_shortcut(const char *bureaublad_pad, const char *home_dir) {
    char shortcut_pad[512];
    snprintf(shortcut_pad, sizeof(shortcut_pad), "%s/LearnLinux.desktop", bureaublad_pad);

    FILE *f = fopen(shortcut_pad, "w");
    if (f != NULL) {
        fprintf(f, "[Desktop Entry]\n");
        fprintf(f, "Version=1.0\n");
        fprintf(f, "Type=Application\n");
        fprintf(f, "Name=Learn Linux\n");
        fprintf(f, "Comment=Start het Learn Linux Project\n");
        fprintf(f, "Exec=bash -c 'cd %s/LearnLinux && ./menu'\n", home_dir);
        fprintf(f, "Icon=%s/LearnLinux/icon.svg\n", home_dir);
        fprintf(f, "Terminal=false\n");
        fprintf(f, "Categories=Education;\n");
        fclose(f);

        // 1. Maak de snelkoppeling uitvoerbaar
        char chmod_cmd[1024];
        snprintf(chmod_cmd, sizeof(chmod_cmd), "chmod +x '%s'", shortcut_pad);
        system(chmod_cmd);

        // 2. Vertel Linux Mint direct dat dit bestand ELKE keuring overleeft en vertrouwd is
        char trust_cmd[1024];
        snprintf(trust_cmd, sizeof(trust_cmd), "gio set '%s' metadata::trusted true 2>/dev/null", shortcut_pad);
        system(trust_cmd);
        
        // Extra zetje voor dbus om de metadata direct te laden
        char dbus_cmd[1024];
        snprintf(dbus_cmd, sizeof(dbus_cmd), "dbus-send --type=method_call --dest=org.freedesktop.FileManager1 /org/freedesktop/FileManager1 org.freedesktop.FileManager1.ShowFolders array:string:\"file://%s\" string:\"\" 2>/dev/null", bureaublad_pad);
        system(dbus_cmd);
    }
}

gboolean do_actual_installation(gpointer user_data) {
    InstallerData *data = (InstallerData *)user_data;
    
    // Stap 1: Map aanmaken in persoonlijke map
    gtk_label_set_text(GTK_LABEL(data->label), "Map aanmaken in je persoonlijke map...");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->progress_bar), 0.15);
    while (gtk_events_pending()) gtk_main_iteration();
    
    system("mkdir -p ~/LearnLinux 2>/dev/null");
    sleep(1); 

    // Stap 2: Downloaden
    gtk_label_set_text(GTK_LABEL(data->label), "Pakket downloaden van internettuner.nl...");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->progress_bar), 0.40);
    while (gtk_events_pending()) gtk_main_iteration();

    int download_res = system("curl -s -L -o /tmp/learnlinux.zip https://internettuner.nl/lucas/learnlinux.zip");
    
    if (download_res != 0) {
        gtk_label_set_text(GTK_LABEL(data->label), "Fout: Downloaden mislukt. Geen internetverbinding?");
        gtk_widget_set_sensitive(data->install_button, TRUE);
        gtk_button_set_label(GTK_BUTTON(data->install_button), "Opnieuw");
        return FALSE; 
    }

    // Stap 3: Uitpakken
    gtk_label_set_text(GTK_LABEL(data->label), "Bestanden uitpakken in ~/LearnLinux...");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->progress_bar), 0.70);
    while (gtk_events_pending()) gtk_main_iteration();

    system("unzip -o -q /tmp/learnlinux.zip -d ~/LearnLinux/ 2>/dev/null");
    sleep(1);

    // STAP 4: SNELKOPPELING MAKEN EN AUTOMATISCH VERTROUWEN
    gtk_label_set_text(GTK_LABEL(data->label), "Snelkoppeling aanmaken op het bureaublad...");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->progress_bar), 0.90);
    while (gtk_events_pending()) gtk_main_iteration();

    const gchar *home_dir = g_get_home_dir();
    char path_buffer[512];

    snprintf(path_buffer, sizeof(path_buffer), "%s/Bureaublad", home_dir);
    maak_bureaublad_shortcut(path_buffer, home_dir);

    snprintf(path_buffer, sizeof(path_buffer), "%s/Desktop", home_dir);
    maak_bureaublad_shortcut(path_buffer, home_dir);

    // STAP 5: AUTOMATISCH HET BUREAUBLAD VERVERSEN (DE FIX!)
    gtk_label_set_text(GTK_LABEL(data->label), "Bureaublad verversen...");
    
    // We herstarten nemo-desktop heel snel op de achtergrond. 
    // Hierdoor herlaadt Cinnamon het bureaublad en pakt hij het icoon DIRECT op!
    system("nemo-desktop --quit && nemo-desktop & 2>/dev/null");
    sleep(1);

    // Stap 6: Voltooid en menu starten
    gtk_label_set_text(GTK_LABEL(data->label), "Installatie succesvol voltooid!");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->progress_bar), 1.0);
    
    system("chmod +x ~/LearnLinux/menu 2>/dev/null");
    system("cd ~/LearnLinux && ./menu & 2>/dev/null");

    // Knop ombouwen naar Sluiten
    gtk_widget_set_sensitive(data->install_button, TRUE);
    gtk_button_set_label(GTK_BUTTON(data->install_button), "Sluiten");
    
    g_signal_handlers_disconnect_by_func(data->install_button, G_CALLBACK(on_install_clicked), data);
    g_signal_connect_swapped(data->install_button, "clicked", G_CALLBACK(gtk_widget_destroy), data->dialog);

    return FALSE; 
}

void on_install_clicked(GtkButton *button, gpointer user_data) {
    InstallerData *data = (InstallerData *)user_data;
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); 
    g_idle_add(do_actual_installation, data);
}

int main(int argc, char *argv[]) {
    InstallerData data;
    gtk_init(&argc, &argv);

    data.dialog = gtk_dialog_new_with_buttons("Learn Linux Installer", NULL, GTK_DIALOG_MODAL, NULL, NULL);
    gtk_window_set_default_size(GTK_WINDOW(data.dialog), 450, 180);
    gtk_window_set_position(GTK_WINDOW(data.dialog), GTK_WIN_POS_CENTER);

    data.install_button = gtk_dialog_add_button(GTK_DIALOG(data.dialog), "Installeren", GTK_RESPONSE_ACCEPT);
    GtkWidget *cancel_button = gtk_dialog_add_button(GTK_DIALOG(data.dialog), "Annuleren", GTK_RESPONSE_REJECT);

    data.label = gtk_label_new("Welkom bij Learn Linux. Er wordt 98.7KB data naar je PC gedownload. Klik op Installeren om te beginnen.");
    gtk_label_set_line_wrap(GTK_LABEL(data.label), TRUE); 
    
    data.progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data.progress_bar), 0.0);
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(data.progress_bar), TRUE);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(data.dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(box), 20);
    
    gtk_box_pack_start(GTK_BOX(box), data.label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), data.progress_bar, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(content_area), box);

    g_signal_connect(data.install_button, "clicked", G_CALLBACK(on_install_clicked), &data);
    g_signal_connect_swapped(cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), data.dialog);
    g_signal_connect(data.dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(data.dialog);
    gtk_main();
    return 0;
}
