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
            fusionnerSommet(abr, abrIteSuiv, abrIteSuiv->borneInf-1);
            return abr;
        }
        else if (element == abrIteSuiv->borneSup + 1){
            abrIteSuiv->borneSup++;
            fusionnerSommet(abr, abrIteSuiv, abrIteSuiv->borneSup+1);
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

T_Sommet* rechercherElementAvecPere(T_Arbre abr, int element, T_Sommet **pere) {
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
    if (abr == NULL) return;
    afficherSommet(abr->filsGauche);
    printf("[%d:%d], ", abr->borneInf, abr->borneSup);
    afficherSommet(abr->filsDroit);
}

void afficherElement(T_Arbre abr) {
    if (abr == NULL) return;
    afficherElement(abr->filsGauche);
    if (abr->borneInf == abr->borneSup) {
        printf("%d, ", abr->borneInf);
    } else {
        for (int i = abr->borneInf ; i <= abr->borneSup ; i++) {
            printf("%d, ", i);
        }
    }
    afficherElement(abr->filsDroit);
}

T_Arbre supprimerRacine(T_Arbre abr) {
    if (abr->filsDroit == NULL && abr->filsGauche == NULL) {
        free(abr);
        return NULL;
    }

    if (abr->filsDroit == NULL) {
        T_Arbre nouv_abr = abr->filsGauche;
        free(abr);
        return nouv_abr;
    }
    if (abr->filsGauche == NULL) {
        T_Arbre nouv_abr = abr->filsDroit;
        free(abr);
        return nouv_abr;
    }
    else {
        T_Sommet *pereSuccsesseur = NULL;
        T_Sommet *succsesseurCible = minimumAvecPere(abr->filsDroit, &pereSuccsesseur);
        if (pereSuccsesseur == NULL) pereSuccsesseur = abr;
        abr->borneInf = succsesseurCible->borneInf;
        abr->borneSup = succsesseurCible->borneSup;
        supprimerNoeud(succsesseurCible, &pereSuccsesseur);
        return abr;
    }
}

void supprimerNoeud(T_Sommet *cible, T_Sommet **pere) {
    if (cible->filsDroit == NULL && cible->filsGauche == NULL) {
        if (cible == (*pere)->filsGauche) (*pere)->filsGauche = NULL;
        else (*pere)->filsDroit = NULL;
        free(cible);
        return;
    }

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
        T_Sommet **pereSuccsesseur = malloc(sizeof(T_Sommet *));
        T_Sommet *succsesseurCible = minimumAvecPere(cible->filsDroit, pereSuccsesseur);
        if (*pereSuccsesseur == NULL) *pereSuccsesseur = cible;
        cible->borneInf = succsesseurCible->borneInf;
        cible->borneSup = succsesseurCible->borneSup;
        supprimerNoeud(succsesseurCible, pereSuccsesseur);
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
    T_Sommet *sommetCible = rechercherElementAvecPere(abr, element, &pereCible);

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
            separerIntervalle(sommetCible, element);
            return abr;
        }
    }
    //--Cas ou l'intervalle est réduit à un unique élément--//
    else {
        if (pereCible == NULL) {
            return supprimerRacine(abr);
        }
        supprimerNoeud(sommetCible, &pereCible);
        return abr;
    }
}

unsigned long tailleMemoire(T_Arbre abr, unsigned long *tailleReel, unsigned long *tailleClassique) {
    *tailleReel = sizeof(T_Sommet) * nombreNoeuds(abr);
    *tailleClassique = sizeof(Classique) * nombreElements(abr);
    return *tailleClassique - *tailleReel;
}

void fusionnerSommet(T_Arbre abr, T_Sommet *cible, int element) {
    if (abr == NULL) return;

    T_Sommet **cible2Pere = malloc(sizeof (T_Sommet*));
    T_Sommet *cible2 = rechercherElementAvecPere(abr, element, cible2Pere);

    if (cible2 == NULL) return;

    if (cible->borneSup + 1 == cible2->borneInf) {
        cible->borneSup = cible2->borneSup;
    }
    else if (cible2->borneSup + 1 == cible->borneInf) {
        cible->borneInf = cible2->borneInf;
    }
    supprimerNoeud(cible2, cible2Pere);
}

int hauteurArbre(T_Arbre abr) {
    if (abr == NULL) return -1;

    int tailleG = hauteurArbre(abr->filsGauche);
    int tailleD = hauteurArbre(abr->filsDroit);

    if (tailleG > tailleD) return tailleG + 1;
    return tailleD + 1;
}

T_Sommet* minimumAvecPere(T_Arbre abr, T_Sommet **pere) {
    *pere = NULL;
    if (abr == NULL) return NULL;

    while (abr->filsGauche != NULL) {
        *pere = abr;
        abr = abr->filsGauche;
    }
    return abr;
}

int nombreNoeuds(T_Arbre abr) {
    if (abr == NULL) return 0;

    return nombreNoeuds(abr->filsGauche) + nombreNoeuds(abr->filsDroit) + 1;
}

int nombreElements(T_Arbre abr) {
    if (abr == NULL) return 0;
    return nombreElements(abr->filsGauche) + nombreElements(abr->filsDroit) + abr->borneSup - abr->borneInf + 1;
}

void libererArbre(T_Arbre abr) {
    if (abr == NULL) return;

    libererArbre(abr->filsGauche);
    libererArbre(abr->filsDroit);
    free(abr);
}