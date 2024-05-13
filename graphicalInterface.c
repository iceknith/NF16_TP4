#include "graphicalInterface.h"

int graphicalMain(int argc, char* argv[]){

    mainArbre = NULL;
    gtk_init (&argc, &argv);

    GtkApplication *app = gtk_application_new("org.gtk.nf16_tp_graphe", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate",G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    gtk_main();
    g_object_unref(app);

    return status;
}

static void activate(){
    //La fonction chargée de l'initialisation de la partie graphique du TP4

    GtkBuilder *builder;
    GObject *window;
    GError *error = NULL;

    //On initialise le builder, qui va prendre les données xml de builder.ui, et qui va les convertir en widgets gtk
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "../builder.ui", &error) == 0){

        //Si une erreur est survenue
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    //On récupère les éléments initialisés
    window = gtk_builder_get_object(builder, "window");
    pop = (GtkPopover*) gtk_builder_get_object(builder, "text_popover");

    //On "lie" les fonctions définies dans le builder.ui, aux Callbacks du code
    gtk_builder_add_callback_symbols(builder,
                                     "inserer_elements", G_CALLBACK(inserer_elements),
                                     "rechercher_element", G_CALLBACK(rechercher_element),
                                     "supprimer_element", G_CALLBACK(supprimer_element),
                                     "valider_clicked", G_CALLBACK(valider_clicked),
                                     "draw_event", G_CALLBACK(draw_event),
                                     "filtre_nombre", G_CALLBACK(filtre_nombre),
                                     "gtk_main_quit", G_CALLBACK(quitter),
                                     NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(GTK_WIDGET(window));
    //gtk_window_maximize(GTK_WINDOW(window));

    //On supprime le builder, car on n'en as plus besoin
    g_object_unref(builder);
}

static void inserer_elements(GtkWidget *widget, gpointer data){

    state = 1;
    counter = -1;

    //Changement du placeholder text
    GtkGrid *g = GTK_GRID(gtk_bin_get_child(GTK_BIN(pop)));
    GtkEntry *e = GTK_ENTRY(gtk_grid_get_child_at(g,0, 1));
    GtkLabel *l = GTK_LABEL(gtk_grid_get_child_at(g,0, 0));
    gtk_entry_buffer_delete_text(gtk_entry_get_buffer(e), 0, gtk_entry_get_text_length(e));
    gtk_entry_set_placeholder_text(e,"Combien de nombres voulez-vous insérer ?");
    gtk_label_set_text(l, "Combien de nombres voulez-vous insérer dans l'arbre ?");

    //Changement du widget vers lequel le popup va pointer
    GtkWidget *button = gtk_grid_get_child_at(
            GTK_GRID(gtk_grid_get_child_at(data, 0, 0)), 0, 0);
    gtk_popover_set_relative_to(pop, button);

    gtk_popover_popup(pop);
}

static void rechercher_element(GtkWidget *widget, gpointer data){
    g_print("Recherche d'un élement\n");
}

static void supprimer_element(GtkWidget *widget, gpointer data){
    g_print("Supression d'un élément\n");
}

static void valider_clicked(GtkWidget *widget, gpointer data){

    GtkGrid *g = GTK_GRID(gtk_bin_get_child(GTK_BIN(pop)));
    GtkEntry *e = GTK_ENTRY(gtk_grid_get_child_at(g,0, 1));
    GtkLabel *l = GTK_LABEL(gtk_grid_get_child_at(g,0, 0));
    GtkWidget *canvas = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(
                    GTK_BIN(gtk_grid_get_child_at(data, 0, 2)))));

    if (state == 1){
        if (counter == -1){
            counter = pop_entry_get_text_value();

            gtk_entry_set_placeholder_text(e,"Quels sont les nombres que vous voulez insérer dans l'arbre ?");
            gtk_label_set_text(l, "Entrez un nombre à insérer dans l'arbre");

        }
        else {
            counter--;
            mainArbre = insererElement(mainArbre, pop_entry_get_text_value());
        }

        if (counter == 0){
            state = 0;
            gtk_popover_popdown(pop);
        }

        gtk_widget_queue_draw(canvas);
        gtk_entry_buffer_delete_text(gtk_entry_get_buffer(e), 0, gtk_entry_get_text_length(e));
    }
}

static void draw_event(GtkWidget *widget, cairo_t *cr, gpointer data){
    if (mainArbre != NULL){
        int h = hauteurArbre(mainArbre) + 1;
        gint width = TEXT_DISTANCE_X * pow(2,  h) + 2 * MARGIN_X;
        gint height = TEXT_DISTANCE_Y * h;

        //Ajuster la taille du canvas
        if (width != prevCanvasWidth){
            GtkAdjustment *hAdjustment = gtk_scrolled_window_get_hadjustment(data);

            gtk_widget_set_size_request(widget, width, height);
            gtk_adjustment_set_value(hAdjustment, width * 0.5);
            prevCanvasWidth = width;
        }

        dessiner_arbre(cr, mainArbre, (double)gtk_widget_get_allocated_width(widget)/2, 100);
    }
}

static void filtre_nombre(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data){
    //Fonction qui est déclenchée à chaque fois qu'on rentre un caractère dans la zone de texte
    //Elle sert à bloquer le signal qu'un caractère as été tapé dans la zone de texte, si le caractère tapé n'est pas un chiffre

    if (!g_unichar_isdigit(g_utf8_get_char(text))){
        g_signal_stop_emission_by_name(editable, "insert-text");
    }
}

static void quitter(GtkWidget *widget, GdkEvent *event, gpointer data){
    g_print("libération de l'espace");
    gtk_main_quit();
}

//---Utilities functions---//

static int pop_entry_get_text_value(){
    GtkEditable *e = GTK_EDITABLE(gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(pop))), 0, 1));
    int len = gtk_editable_get_position(e);
    if (len == 0) return 0;
    return atoi(gtk_editable_get_chars(e, 0, len));
}

static void dessiner_arbre(cairo_t *cr, T_Arbre arbre, double posX, double posY){
    dessiner_sommet(cr, arbre, posX, posY);

    cairo_set_source_rgb(cr, LINE_R, LINE_G, LINE_B);
    cairo_set_line_width(cr, LINE_WIDTH);

    double maxFeuilles = pow(2, hauteurArbre(arbre));
    double intervalleFeuillesX = TEXT_DISTANCE_X * maxFeuilles;

    if (arbre->filsDroit != NULL){
        dessiner_arbre(cr, arbre->filsDroit, posX + intervalleFeuillesX/2, posY + TEXT_DISTANCE_Y);
        cairo_move_to(cr, posX, posY + TEXT_LINE_DIST_Y);
        cairo_line_to(cr, posX + intervalleFeuillesX/2, posY + TEXT_DISTANCE_Y - TEXT_LINE_DIST_Y);
    }
    if (arbre->filsGauche != NULL){
        dessiner_arbre(cr, arbre->filsGauche, posX - intervalleFeuillesX/2, posY + TEXT_DISTANCE_Y);
        cairo_move_to(cr, posX, posY + TEXT_LINE_DIST_Y);
        cairo_line_to(cr, posX - intervalleFeuillesX/2, posY + TEXT_DISTANCE_Y - TEXT_LINE_DIST_Y);
    }
    cairo_stroke(cr);
}

static void dessiner_sommet(cairo_t *cr, T_Arbre arbre, double posX, double posY){
    if (arbre == NULL) return;

    if (arbre == arbreSelectionne){
        cairo_set_font_size (cr, SELECTED_TXT_SIZE);
        cairo_set_source_rgb(cr, SELECTED_TXT_R, SELECTED_TXT_G, SELECTED_TXT_B);
    }
    else{
        cairo_set_font_size (cr, UNSELECTED_TXT_SIZE);
        cairo_set_source_rgb(cr, UNSELECTED_TXT_R, UNSELECTED_TXT_G, UNSELECTED_TXT_B);
    }

    cairo_text_extents_t te;
    char text[9];
    sprintf(text, "[%d:%d]", arbre->borneInf, arbre->borneSup);

    cairo_text_extents (cr, text, &te);
    cairo_move_to (cr,
                   posX - te.x_bearing - te.width / 2,
                   posY - te.y_bearing - te.height / 2);
    cairo_show_text (cr, text);
}