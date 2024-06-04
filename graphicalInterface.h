#ifndef NF16_TP4_GRAPHICALINTERFACE_H
#define NF16_TP4_GRAPHICALINTERFACE_H

#include <gtk/gtk.h>
#include <math.h>
#include "tp4.c"

#define TEXT_DISTANCE_X 100
#define TEXT_DISTANCE_Y 100
#define MARGIN_X 100

#define LINE_WIDTH 5
#define TEXT_LINE_DIST_Y 25

#define SMALL_TXT_SIZE 25
#define UNSELECTED_TXT_SIZE 40

#define SELECTED_TXT_SIZE 50
#define SMALL_SELECTED_TXT_SIZE 40
#define SELECTED_TXT_R 0.9
#define SELECTED_TXT_G 0
#define SELECTED_TXT_B 0

#define GUI_TEXT_MARGIN_X 50
#define GUI_TEXT_MARGIN_Y 50
#define GUI_TEXT_DIST_X 30

GtkPopover *pop;
gint prevCanvasWidth;
struct _GdkRGBA *unselectedColor = NULL;

T_Arbre mainArbre;
int nombreSelectionne = -1;
T_Arbre arbreSelectionne;
int counter = 0;
int state = 0;
//0 -> No state
//1 -> Inserer Elements
//2 -> Rechercher Element
//3 -> Supprimer Element

int graphicalMain(int argc, char* argv[]);
static void activate();
static void inserer_elements(GtkWidget *widget, gpointer data);
static void rechercher_element(GtkWidget *widget, gpointer data);
static void supprimer_element(GtkWidget *widget, gpointer data);
static void valider_clicked(GtkWidget *widget, gpointer data);
static void draw_main_canvas_event(GtkWidget *widget, cairo_t *cr, gpointer data);
static void draw_gui_canvas_event(GtkWidget *widget, cairo_t *cr, gpointer data);
static void filtre_nombre(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data);
static void text_entry_activate(GtkEditable *editable, gpointer *data);
static void quitter(GtkWidget *widget, GdkEvent *event, gpointer data);

//---Utilities functions---//
static int pop_entry_get_text_value();
static void dessiner_liste_element(cairo_t *cr, T_Arbre arbre, double* posX, double posY);
static void dessiner_arbre(cairo_t *cr, T_Arbre arbre, double posX, double posY);
static void dessiner_sommet(cairo_t *cr, T_Arbre arbre, double posX, double posY);

#endif
