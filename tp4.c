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
    //--Cas triviaux--//
    if (abr == NULL) return creerSommet(element);

    if (rechercherElement(abr, element) != NULL) return abr;

    //--Cas non-trivial--//
    //On trouve la position d'insertion du nouveau sommet
    T_Arbre abrIteSuiv = abr;
    T_Arbre abrIte;
    while (abrIteSuiv != NULL) {
        abrIte = abrIteSuiv;

        //Si element est consecutif a un ensemble, on change les bornes de l'ensemble
        if (element == abrIteSuiv->borneInf - 1){
            abrIteSuiv->borneInf--;
            return abr;
        }
        else if (element == abrIteSuiv->borneSup + 1){
            abrIteSuiv->borneSup++;
            return abr;
        }
        //Sinon, on précise notre recherche
        else if (element < abrIteSuiv->borneInf) {
            abrIteSuiv = abrIteSuiv->filsGauche;
        }
        else {
            abrIteSuiv = abrIteSuiv->filsDroit;
        }
    }

    //On insère le nouveau sommet
    T_Sommet *nouvSommet = creerSommet(element);
    if (element < abrIte->borneInf) {
        abrIte->filsGauche = nouvSommet;
    }
    else {
        abrIte->filsDroit = nouvSommet;
    }

    return abr;
}

T_Sommet* rechercherElement(T_Arbre abr, int element) {
    if (abr == NULL) return NULL;

    while (abr != NULL) {
        if (element >= abr->borneInf && element <= abr->borneSup) {
            return abr;
        }
        else if (element < abr->borneInf) {
            abr = abr->filsDroit;
        }
        else {
            abr = abr->filsGauche;
        }
    }

    return NULL;
}

void afficherSommet(T_Arbre abr) {

}

void afficherElement(T_Arbre abr) {

}

T_Arbre supprimerElement(T_Arbre abr, int element) {

}

void tailleMemoire(T_Arbre abr) {

}



int hauteurArbre(T_Arbre abr) {
    if (abr == NULL) return -1;

    int tailleG = hauteurArbre(abr->filsGauche);
    int tailleD = hauteurArbre(abr->filsDroit);

    if (tailleG > tailleD) return tailleG + 1;
    return tailleD + 1;
}
