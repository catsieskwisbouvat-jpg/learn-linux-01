#include <gtk/gtk.h>

// Globale variabelen om de keuzerondjes te controleren
GtkWidget *radio_correct;
GtkWidget *radio_wrong1;
GtkWidget *radio_wrong2;
GtkWidget *window;

// Deze functie controleert het antwoord als je op de controleer-knop drukt
void on_check_answer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    
    // Check of de juiste radio button is geselecteerd
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_correct))) {
        dialog = gtk_message_dialog_new(
            GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Goed gedaan! 🎉\n\nDe Software Manager (Programmabeheer) is inderdaad de veiligste plek om software te installeren."
        );
        gtk_window_set_title(GTK_WINDOW(dialog), "Goed Antwoord!");
    } else {
        dialog = gtk_message_dialog_new(
            GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Helaas, dat is niet juist. ❌\n\nTip: Kijk nog eens op tabblad 2 ('Software')!"
        );
        gtk_window_set_title(GTK_WINDOW(dialog), "Fout Antwoord");
    }
    
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Functie die geprint wordt in de terminal bij het switchen van tabbladen
void on_tab_changed(GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data) {
    g_print("Gebruiker switchte naar tabblad nummer: %d\n", page_num + 1);
}

int main(int argc, char *argv[]) {
    GtkWidget *notebook;
    GtkWidget *vbox;
    GtkWidget *title_label;
    GtkWidget *btn_close;
    
    // Pagina's en labels
    GtkWidget *page1, *page2, *page3, *page4;
    GtkWidget *label1, *label2, *label3, *label4;
    GtkWidget *btn_check;

    gtk_init(&argc, &argv);

    // 1. Hoofdvenster aanmaken
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Les: De Basis van Linux Mint");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 420);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), "<span size='large' weight='bold' foreground='#41a62a'>Leren: De Basis</span>");
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 5);

    // 2. Notebook aanmaken
    notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

    // --- TABBLAD 1: Het Bureaublad ---
    page1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page1), 10);
    label1 = gtk_label_new(
        "Welkom bij Linux Mint!\n\n"
        "Linux Mint gebruikt standaard de 'Cinnamon' omgeving.\n"
        "Dit lijkt erg op Windows: je hebt linksonder een startmenu,\n"
        "een taakbalk onderaan en icoontjes op je bureaublad.\n\n"
        "Klik op het volgende tabblad om meer te leren!"
    );
    gtk_label_set_line_wrap(GTK_LABEL(label1), TRUE);
    gtk_box_pack_start(GTK_BOX(page1), label1, TRUE, TRUE, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page1, gtk_label_new("1. Het Bureaublad"));

    // --- TABBLAD 2: Programmabeheer ---
    page2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page2), 10);
    label2 = gtk_label_new(
        "Hoe installeer je programma's?\n\n"
        "In Linux Mint download je software niet van websites.\n"
        "Je gebruikt de 'Software Manager' (Programmabeheer).\n"
        "Dit werkt net als de App Store of Play Store:\n"
        "veilig, snel en met één klik geïnstalleerd!"
    );
    gtk_label_set_line_wrap(GTK_LABEL(label2), TRUE);
    gtk_box_pack_start(GTK_BOX(page2), label2, TRUE, TRUE, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page2, gtk_label_new("2. Software"));

    // --- TABBLAD 3: Updates ---
    page3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page3), 10);
    label3 = gtk_label_new(
        "Veiligheid en Updates:\n\n"
        "Rechtsonder in de taakbalk zie je een schild-icoontje.\n"
        "Dit is de Update Manager.\n"
        "Linux Mint valt je nooit lastig met onverwachte herstarts.\n"
        "Jij kiest zélf wanneer je updates uitvoert!"
    );
    gtk_label_set_line_wrap(GTK_LABEL(label3), TRUE);
    gtk_box_pack_start(GTK_BOX(page3), label3, TRUE, TRUE, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page3, gtk_label_new("3. Updates"));

    // --- TABBLAD 4: INTERACTIEVE OPDRACHT (PROBEER HET ZELF) ---
    page4 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(page4), 15);
    
    label4 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label4), "<b>Vraag: Wat is de veiligste manier om nieuwe software te installeren in Linux Mint?</b>");
    gtk_label_set_line_wrap(GTK_LABEL(label4), TRUE);
    gtk_box_pack_start(GTK_BOX(page4), label4, FALSE, FALSE, 5);

    // Maak de meerkeuzerondjes (Radio Buttons) aan
    radio_wrong1 = gtk_radio_button_new_with_label(NULL, "A) Zoeken op Google en een .exe bestand downloaden.");
    gtk_box_pack_start(GTK_BOX(page4), radio_wrong1, FALSE, FALSE, 2);

    // Koppel de andere rondjes aan de groep van de eerste knop
    radio_correct = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_wrong1), "B) Via de ingebouwde Software Manager (Programmabeheer).");
    gtk_box_pack_start(GTK_BOX(page4), radio_correct, FALSE, FALSE, 2);

    radio_wrong2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_wrong1), "C) Helemaal geen programma's installeren, dat kan niet op Linux.");
    gtk_box_pack_start(GTK_BOX(page4), radio_wrong2, FALSE, FALSE, 2);

    // Controleer knop voor de opdracht
    btn_check = gtk_button_new_with_label("Controleer mijn antwoord!");
    gtk_box_pack_start(GTK_BOX(page4), btn_check, FALSE, FALSE, 10);
    g_signal_connect(btn_check, "clicked", G_CALLBACK(on_check_answer_clicked), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page4, gtk_label_new("4. Probeer het zelf!"));

    // Schakel tabblad-detectie in
    g_signal_connect(notebook, "switch-page", G_CALLBACK(on_tab_changed), NULL);

    // Sluitknop onderaan
    btn_close = gtk_button_new_with_label("Les afronden");
    g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_box_pack_start(GTK_BOX(vbox), btn_close, FALSE, FALSE, 5);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
