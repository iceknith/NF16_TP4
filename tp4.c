#include <stdio.h>
#include <stdlib.h>
#include "tp4.h"

T_Sommet* creerSommet(int element) {
    T_Sommet *newSommet = malloc(sizeof(T_Sommet));
    newSommet->borneInf = element;
    newSommet->borneSup = element;
    newSommet->filsDroit = NULL;
    newSommet->filsGauche = NULL;
    return newSommet;
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

        if (element < abr->borneInf) {
            abr = abr->filsGauche;
        }
        else {
            abr = abr->filsDroit;
        }
    }

    return NULL;
}

T_Sommet* rechercherBis(T_Arbre abr, int element, T_Sommet *pere) {
    pere = NULL;
    if (abr == NULL) return NULL;

    while (abr != NULL) {
        if (element >= abr->borneInf && element <= abr->borneSup) {
            return abr;
        }

        pere = abr;
        if (element < abr->borneInf) {
            abr = abr->filsGauche;
        }
        else {
            abr = abr->filsDroit;
        }
    }
    pere = NULL;
    return NULL;
}

void afficherSommet(T_Arbre abr) {

}

void afficherElement(T_Arbre abr) {

}

T_Arbre supprimerElement(T_Arbre abr, int element) {
    if (abr == NULL) return NULL;

    T_Sommet *pereCible = NULL;
    T_Sommet *sommetCible = rechercherBis(abr, element, pereCible);

    if (sommetCible == NULL) return abr;

    //--Cas où l'intervalle n'est pas réduit à un élément--//
    //Cas ou l'éléments est l'une des bornes de l'intervalle
    if (element == sommetCible->borneInf) {
        sommetCible->borneInf ++;
        return abr;
    }
    else if (element == sommetCible->borneSup) {
        sommetCible->borneSup--;
        return abr;
    }
    //Cas ou l'élément est contenue dans l'intervalle
    if (sommetCible->borneInf < element && element < sommetCible->borneSup) {
        if (hauteurArbre(sommetCible->filsGauche) <= hauteurArbre(sommetCible->filsDroit)) {
            T_Sommet *newSommet = creerSommet(sommetCible->borneInf);
            newSommet->borneSup = element - 1;
            newSommet->filsGauche = sommetCible->filsGauche;
            sommetCible->borneInf = element + 1;
            sommetCible->filsGauche = newSommet;
            return abr;
        }
        else {
            T_Sommet *newSommet = creerSommet(sommetCible->borneSup);
            newSommet->borneInf = element + 1;
            newSommet->filsDroit = sommetCible->filsDroit;
            sommetCible->borneSup = element - 1;
            sommetCible->filsDroit = newSommet;
            return abr;
        }
    }

    //--Cas ou l'intervalle est réduit à un unique élément--//
    if (sommetCible->filsDroit == NULL) {
        if (pereCible->filsGauche == sommetCible) {
            pereCible->filsGauche = sommetCible->filsGauche;
            free(sommetCible);
            return abr;
        }
        else {
            pereCible->filsDroit = sommetCible->filsGauche;
            free(sommetCible);
            return abr;
        }
    }
    else if (sommetCible->filsGauche == NULL) {
        if (pereCible->filsGauche == sommetCible) {
            pereCible->filsGauche = sommetCible->filsDroit;
            free(sommetCible);
            return abr;
        }
        else {
            pereCible->filsDroit = sommetCible->filsDroit;
            free(sommetCible);
            return abr;
        }
    }
    else {
        T_Sommet *pereSuccsesseur = NULL;
        T_Sommet *succsesseurCible = minimum(sommetCible, pereSuccsesseur);
        pereSuccsesseur->filsGauche = succsesseurCible->filsDroit;
        sommetCible->borneInf = succsesseurCible->borneInf;
        sommetCible->borneSup = succsesseurCible->borneSup;
        free(succsesseurCible);
        return abr;
    }
}

unsigned long tailleMemoire(T_Arbre abr) {
    return sizeof(T_Sommet) * nombreNoeuds(abr);
}



int hauteurArbre(T_Arbre abr) {
    if (abr == NULL) return -1;

    int tailleG = hauteurArbre(abr->filsGauche);
    int tailleD = hauteurArbre(abr->filsDroit);

    if (tailleG > tailleD) return tailleG + 1;
    return tailleD + 1;
}

T_Sommet *minimum(T_Arbre abr, T_Sommet *pere) {
    pere = NULL;
    if (abr == NULL) return NULL;

    while (abr->filsDroit != NULL) {
        pere = abr;
        abr = abr->filsDroit;
    }
    return abr;
}
int nombreNoeuds(T_Arbre abr) {
    if (abr == NULL) return 0;

    return nombreNoeuds(abr->filsGauche) + nombreNoeuds(abr->filsDroit) + 1;
}