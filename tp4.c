#include <stdio.h>
#include <stdlib.h>
#include "tp4.h"

T_Sommet* creerSommet(int element) {
    T_Sommet *new_sommet = malloc(sizeof(T_Sommet));
    new_sommet->borneInf = element;
    new_sommet->borneSup = element;
    new_sommet->filsDroit = NULL;
    new_sommet->filsGauche = NULL;
    return new_sommet;
}

T_Arbre insererElement(T_Arbre abr, int element) {

}

T_Sommet* rechercherElement(T_Arbre abr, int element) {
    if (abr == NULL) return NULL;

    while (abr != NULL) {
        if (element >= abr->borneInf && element <= abr->borneSup) {
            return abr;
        } else if (element < abr->borneInf) {
            abr = abr->filsDroit;
            continue;
        } else {
            abr = abr->filsGauche;
        }
    }

    return NULL;
}

void afficherSommet(T_Arbre abr) {
    return;
}

void afficherElement(T_Arbre abr) {
    return;
}

T_Arbre supprimerElement(T_Arbre abr, int element) {

}

void tailleMemoire(T_Arbre abr) {
    return;
}
