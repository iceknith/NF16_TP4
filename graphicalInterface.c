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
    if (gtk_builder_add_from_file(builder, "./builder.ui", &error) == 0){
        //Si une erreur est survenue
        g_printerr("Error loading file: %styleContext\n", error->message);
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
                                     "draw_main_canvas_event", G_CALLBACK(draw_main_canvas_event),
                                     "draw_gui_canvas_event", G_CALLBACK(draw_gui_canvas_event),
                                     "filtre_nombre", G_CALLBACK(filtre_nombre),
                                     "text_entry_activate", G_CALLBACK(text_entry_activate),
                                     "gtk_main_quit", G_CALLBACK(quitter),
                                     NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_window_maximize(GTK_WINDOW(window));

    //On trouve la couleur de base du système pour l'utiliser lors du dessin de l'arbre
    GtkStyleContext *styleContext = gtk_widget_get_style_context(GTK_WIDGET(window));
    //Initialise color
    unselectedColor = malloc(sizeof(struct _GdkRGBA));
    unselectedColor->alpha = 0;
    unselectedColor->red = 0;
    unselectedColor->green = 0;
    unselectedColor->blue = 0;

    gtk_style_context_get_color(styleContext, GTK_STATE_FLAG_NORMAL, unselectedColor);

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
    gtk_popover_set_relative_to(pop, widget);

    //Raffraichir les canvas
    GtkWidget *main_canvas = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(
            GTK_BIN(gtk_grid_get_child_at(data, 0, 2)))));
    GtkWidget *gui_canvas = gtk_grid_get_child_at(data, 0, 1);
    gtk_widget_queue_draw(main_canvas);
    gtk_widget_queue_draw(gui_canvas);

    gtk_popover_popup(pop);
}

static void rechercher_element(GtkWidget *widget, gpointer data){
    state = 2;
    counter = -1;

    //Changement du placeholder text
    GtkGrid *g = GTK_GRID(gtk_bin_get_child(GTK_BIN(pop)));
    GtkEntry *e = GTK_ENTRY(gtk_grid_get_child_at(g,0, 1));
    GtkLabel *l = GTK_LABEL(gtk_grid_get_child_at(g,0, 0));
    gtk_entry_buffer_delete_text(gtk_entry_get_buffer(e), 0, gtk_entry_get_text_length(e));
    gtk_entry_set_placeholder_text(e,"Quel élément voulez-vous chercher ?");
    gtk_label_set_text(l, "Quel élément voulez-vous chercher ?");

    //Changement du widget vers lequel le popup va pointer
    gtk_popover_set_relative_to(pop, widget);

    //Raffraichir les canvas
    GtkWidget *main_canvas = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(
            GTK_BIN(gtk_grid_get_child_at(data, 0, 2)))));
    GtkWidget *gui_canvas = gtk_grid_get_child_at(data, 0, 1);
    gtk_widget_queue_draw(main_canvas);
    gtk_widget_queue_draw(gui_canvas);

    gtk_popover_popup(pop);
}

static void supprimer_element(GtkWidget *widget, gpointer data){
    state = 3;
    counter = -1;

    //Changement du placeholder text
    GtkGrid *g = GTK_GRID(gtk_bin_get_child(GTK_BIN(pop)));
    GtkEntry *e = GTK_ENTRY(gtk_grid_get_child_at(g,0, 1));
    GtkLabel *l = GTK_LABEL(gtk_grid_get_child_at(g,0, 0));
    gtk_entry_buffer_delete_text(gtk_entry_get_buffer(e), 0, gtk_entry_get_text_length(e));
    gtk_entry_set_placeholder_text(e,"Combien d'éléments voulez-vous supprimer ?");
    gtk_label_set_text(l, "Combien d'éléments voulez-vous supprimer ?");

    //Changement du widget vers lequel le popup va pointer
    gtk_popover_set_relative_to(pop, widget);

    //Raffraichir les canvas
    GtkWidget *main_canvas = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(
            GTK_BIN(gtk_grid_get_child_at(data, 0, 2)))));
    GtkWidget *gui_canvas = gtk_grid_get_child_at(data, 0, 1);
    gtk_widget_queue_draw(main_canvas);
    gtk_widget_queue_draw(gui_canvas);

    gtk_popover_popup(pop);
}

static void valider_clicked(GtkWidget *widget, gpointer data){

    GtkGrid *g = GTK_GRID(gtk_bin_get_child(GTK_BIN(pop)));
    GtkEntry *e = GTK_ENTRY(gtk_grid_get_child_at(g,0, 1));
    GtkLabel *l = GTK_LABEL(gtk_grid_get_child_at(g,0, 0));

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

        gtk_entry_buffer_delete_text(gtk_entry_get_buffer(e), 0, gtk_entry_get_text_length(e));
    }
    else if (state == 2){
        state = 0;

        nombreSelectionne = pop_entry_get_text_value();
        arbreSelectionne = rechercherElement(mainArbre, nombreSelectionne);

        gtk_popover_popdown(pop);
        gtk_entry_buffer_delete_text(gtk_entry_get_buffer(e), 0, gtk_entry_get_text_length(e));
    }
    else if (state == 3){

        if (counter == -1){
            counter = pop_entry_get_text_value();

            gtk_entry_set_placeholder_text(e,"Quels sont les sommets que vous voulez supprimer de l'arbre ?");
            gtk_label_set_text(l, "Entrez un sommet à supprimer de l'arbre");

        }
        else {
            counter--;
            mainArbre = supprimerElement(mainArbre, pop_entry_get_text_value());
        }

        if (counter == 0){
            state = 0;
            gtk_popover_popdown(pop);
        }

        gtk_entry_buffer_delete_text(gtk_entry_get_buffer(e), 0, gtk_entry_get_text_length(e));
    }

    //Raffraichir les canvas
    GtkWidget *main_canvas = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(
            GTK_BIN(gtk_grid_get_child_at(data, 0, 2)))));
    GtkWidget *gui_canvas = gtk_grid_get_child_at(data, 0, 1);
    gtk_widget_queue_draw(main_canvas);
    gtk_widget_queue_draw(gui_canvas);
}

static void draw_main_canvas_event(GtkWidget *widget, cairo_t *cr, gpointer data){

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

static void draw_gui_canvas_event(GtkWidget *widget, cairo_t *cr, gpointer data){

    double* posX = malloc(sizeof(double));
    *posX = GUI_TEXT_MARGIN_X;

    //Dessiner les éléments de la liste
    cairo_set_font_size (cr, SMALL_TXT_SIZE);
    gdk_cairo_set_source_rgba(cr, unselectedColor);
    cairo_text_extents_t te;

    cairo_text_extents (cr, "Voici les éléments de l'arbre: ", &te);
    cairo_move_to (cr, *posX,
                   GUI_TEXT_MARGIN_Y - te.y_bearing - te.height / 2);
    cairo_show_text (cr, "Voici les éléments de l'arbre: ");
    *posX += te.x_bearing + te.width + GUI_TEXT_DIST_X;

    dessiner_liste_element(cr, mainArbre, posX, GUI_TEXT_MARGIN_Y);

    //Dessiner le nombre de place occupé
    *posX += GUI_TEXT_MARGIN_X;
    char text[200];
    unsigned long *tailleReel = malloc(sizeof(unsigned long));
    unsigned long *tailleClassique = malloc(sizeof(unsigned long));
    unsigned long taille = tailleMemoire(mainArbre, tailleReel, tailleClassique);
    sprintf(text, "Taille occupée: %lu octets.  "
                  "Taille occupée par un arbre classique: %lu octets.  "
                  "Gain de place: %lu octets.", *tailleReel, *tailleClassique, taille);
    cairo_set_font_size (cr, SMALL_TXT_SIZE);
    gdk_cairo_set_source_rgba(cr, unselectedColor);

    cairo_text_extents (cr, text, &te);
    double new_posX = gtk_widget_get_allocated_width(widget) - GUI_TEXT_MARGIN_X - te.x_bearing - te.width;

    if (*posX > new_posX){
        cairo_move_to (cr, *posX,GUI_TEXT_MARGIN_Y - te.y_bearing - te.height / 2);
    }
    else {
        cairo_move_to (cr,new_posX,GUI_TEXT_MARGIN_Y - te.y_bearing - te.height / 2);
    }

    cairo_show_text (cr, text);
    *posX += te.x_bearing + te.width + GUI_TEXT_DIST_X;
}

static void filtre_nombre(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data){
    //Fonction qui est déclenchée à chaque fois qu'on rentre un caractère dans la zone de texte
    //Elle sert à bloquer le signal qu'un caractère as été tapé dans la zone de texte, si le caractère tapé n'est pas un chiffre

    if (!g_unichar_isdigit(g_utf8_get_char(text))){
        g_signal_stop_emission_by_name(editable, "insert-text");
    }
}

static void text_entry_activate(GtkEditable *editable, gpointer *data){
    g_signal_emit_by_name(data, "clicked");
}

static void quitter(GtkWidget *widget, GdkEvent *event, gpointer data){
    libererArbre(mainArbre);
    gtk_main_quit();
}

//---Utilities functions---//

static int pop_entry_get_text_value(){
    GtkEditable *e = GTK_EDITABLE(gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(pop))), 0, 1));
    int len = gtk_editable_get_position(e);
    if (len == 0) return 0;
    return atoi(gtk_editable_get_chars(e, 0, len));
}

static void dessiner_liste_element(cairo_t *cr, T_Arbre arbre, double *posX, double posY){
    if (arbre == NULL) return;

    dessiner_liste_element(cr, arbre->filsGauche, posX, posY);

    for (int i = arbre->borneInf; i <= arbre->borneSup; i++){
        if (i == nombreSelectionne){
            cairo_set_font_size (cr, SMALL_SELECTED_TXT_SIZE);
            cairo_set_source_rgb(cr, SELECTED_TXT_R, SELECTED_TXT_G, SELECTED_TXT_B);
        }
        else{
            cairo_set_font_size (cr, SMALL_TXT_SIZE);
            gdk_cairo_set_source_rgba(cr, unselectedColor);
        }

        cairo_text_extents_t te;
        char text[9];
        sprintf(text, "%d", i);

        cairo_text_extents (cr, text, &te);
        cairo_move_to (cr,*posX,posY - te.y_bearing - te.height/2);
        *posX += te.x_bearing + te.width + GUI_TEXT_DIST_X;
        cairo_show_text (cr, text);
    }

    dessiner_liste_element(cr, arbre->filsDroit, posX, posY);
}

static void dessiner_arbre(cairo_t *cr, T_Arbre arbre, double posX, double posY){
    dessiner_sommet(cr, arbre, posX, posY);

    double maxFeuilles = pow(2, hauteurArbre(arbre));
    double intervalleFeuillesX = TEXT_DISTANCE_X * maxFeuilles;

    gdk_cairo_set_source_rgba(cr, unselectedColor);
    cairo_set_line_width(cr, LINE_WIDTH);

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
        gdk_cairo_set_source_rgba(cr, unselectedColor);
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