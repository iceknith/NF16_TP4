#ifndef NF16_TP4_GRAPHICALINTERFACE_H
#define NF16_TP4_GRAPHICALINTERFACE_H

#include <gtk/gtk.h>
#include "tp4.h"

GtkPopover* pop;

int graphicalMain(int argc, char* argv[]);
static void activate();
static void inserer_elements(GtkWidget *widget, gpointer data);
static void rechercher_element(GtkWidget *widget, gpointer data);
static void supprimer_element(GtkWidget *widget, gpointer data);
static void draw_event(GtkWidget *widget, cairo_t *cr, gpointer data);
static void filtre_nombre(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data);

#endif
