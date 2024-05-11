#include "graphicalInterface.h"

int graphicalMain(int argc, char* argv[]){
    gtk_init (&argc, &argv);

    GtkApplication *app = gtk_application_new("org.gtk.nf16_tp_graphe", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate",G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    gtk_main();
    g_object_unref(app);

    return status;
}

static void activate(){
    //La fonction chargée de l'initialisation de la partie graphique du TP3

    GtkBuilder *builder;
    GObject *window;
    GError *error = NULL;

    //On initialise le builder, qui va prendre les données xml de builder.ui, et qui va les convertir en widgets gtk
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "../builder.ui", &error) == 0){

        //Si une erreur est survenue
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return;
    }

    //On récupère les éléments initialisés
    window = gtk_builder_get_object (builder, "window");
    pop = (GtkPopover *) gtk_builder_get_object(builder, "text_popover");

    //On "lie" les fonctions définies dans le builder.ui, aux Callbacks du code
    gtk_builder_add_callback_symbols(builder,
                                     "inserer_elements", G_CALLBACK(inserer_elements),
                                     "rechercher_element", G_CALLBACK(rechercher_element),
                                     "supprimer_element", G_CALLBACK(supprimer_element),
                                     "draw_event", G_CALLBACK(draw_event),
                                     "filtre_nombre", G_CALLBACK(filtre_nombre),
                                     NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_window_maximize(GTK_WINDOW(window));

    //On supprime le builder, car on n'en as plus besoin
    g_object_unref(builder);
}

static void inserer_elements(GtkWidget *widget, gpointer data){
    g_print("Inertion elements\n");
}

static void rechercher_element(GtkWidget *widget, gpointer data){
    g_print("Recherche d'un élement\n");
}

static void supprimer_element(GtkWidget *widget, gpointer data){
    g_print("Supression d'un élément\n");
}

static void draw_event(GtkWidget *widget, cairo_t *cr, gpointer data){
    g_print("dessin\n");
}

static void filtre_nombre(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data){
    //Fonction qui est déclenchée à chaque fois qu'on rentre un caractère dans la zone de texte
    //Elle sert à bloquer le signal qu'un caractère as été tapé dans la zone de texte, si le caractère tapé n'est pas un chiffre

    if (!g_unichar_isdigit(g_utf8_get_char(text))){
        g_signal_stop_emission_by_name(editable, "insert-text");
    }
}