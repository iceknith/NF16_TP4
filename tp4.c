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

T_Sommet* rechercherBis(T_Arbre abr, int element, T_Sommet **pere) {
    *pere = NULL;
    if (abr == NULL) return NULL;

    while (abr != NULL) {
        if (element >= abr->borneInf && element <= abr->borneSup) {
            return abr;
        }

        *pere = abr;
        if (element < abr->borneInf) {
            abr = abr->filsGauche;
        }
        else {
            abr = abr->filsDroit;
        }
    }
    *pere = NULL;
    return NULL;
}

void afficherSommet(T_Arbre abr) {

}

void afficherElement(T_Arbre abr) {

}

void supprimerNoeud(T_Sommet *cible, T_Sommet **pere) {
    if (cible->filsDroit == NULL) {
        if ((*pere)->filsGauche == cible) {
            (*pere)->filsGauche = cible->filsGauche;
            free(cible);
            return;
        }
        else {
            (*pere)->filsDroit = cible->filsGauche;
            free(cible);
            return;
        }
    }
    if (cible->filsGauche == NULL) {
        if ((*pere)->filsGauche == cible) {
            (*pere)->filsGauche = cible->filsDroit;
            free(cible);
            return;
        }
        else {
            (*pere)->filsDroit = cible->filsDroit;
            free(cible);
            return;
        }
    }
    else {
        T_Sommet *pereSuccsesseur = NULL;
        T_Sommet *succsesseurCible = minimum(cible, &pereSuccsesseur);
        cible->borneInf = succsesseurCible->borneInf;
        cible->borneSup = succsesseurCible->borneSup;
        supprimerNoeud(succsesseurCible, &pereSuccsesseur);
    }
}

void separerIntervalle(T_Sommet *cible, int element) {
    if (hauteurArbre(cible->filsGauche) <= hauteurArbre(cible->filsDroit)) {
        T_Sommet *newSommet = creerSommet(cible->borneInf);
        newSommet->borneSup = element - 1;
        newSommet->filsGauche = cible->filsGauche;
        cible->borneInf = element + 1;
        cible->filsGauche = newSommet;
        return;
    }
    else {
        T_Sommet *newSommet = creerSommet(cible->borneSup);
        newSommet->borneInf = element + 1;
        newSommet->filsDroit = cible->filsDroit;
        cible->borneSup = element - 1;
        cible->filsDroit = newSommet;
        return;
    }
}

T_Arbre supprimerElement(T_Arbre abr, int element) {
    if (abr == NULL) return NULL;

    T_Sommet *pereCible = NULL;
    T_Sommet *sommetCible = rechercherBis(abr, element, &pereCible);

    if (sommetCible == NULL) return abr;

    //--Cas où l'intervalle n'est pas réduit à un élément--//
    if (sommetCible->borneSup != sommetCible->borneInf) {
        //Cas ou l'éléments est l'une des bornes de l'intervalle
        if (element == sommetCible->borneInf) {
            sommetCible->borneInf ++;
            return abr;
        }
        if (element == sommetCible->borneSup) {
            sommetCible->borneSup--;
            return abr;
        }
        //Cas ou l'élément est contenue dans l'intervalle
        else {
            separarerIntervalle(sommetCible, element);
            return abr;
        }
    }
    //--Cas ou l'intervalle est réduit à un unique élément--//
    else {
        supprimerNoeud(sommetCible, &pereCible);
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

T_Sommet* minimum(T_Arbre abr, T_Sommet **pere) {
    *pere = NULL;
    if (abr == NULL) return NULL;

    while (abr->filsGauche != NULL) {
        *pere = abr;
        abr = abr->filsDroit;
    }
    return abr;
}
int nombreNoeuds(T_Arbre abr) {
    if (abr == NULL) return 0;

    return nombreNoeuds(abr->filsGauche) + nombreNoeuds(abr->filsDroit) + 1;
}