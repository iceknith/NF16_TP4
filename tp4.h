#ifndef TP4_HEAD
#define TP4_HEAD

typedef struct Sommet T_Sommet;
typedef T_Sommet *T_Arbre;

struct Sommet {
    int borneInf;
    int borneSup;
    struct Sommet *filsGauche;
    struct Sommet *filsDroit;
};

T_Sommet *creerSommet(int element);
T_Arbre insererElement(T_Arbre abr, int element);
T_Sommet *rechercherElement(T_Arbre abr, int element);
void afficherSommet(T_Arbre abr);
void afficherElement(T_Arbre abr);
T_Arbre supprimerElement(T_Arbre abr, int element);
void tailleMemoire(T_Arbre abr);

T_Sommet *rechercherBis(T_Arbre abr, int element, T_Sommet *pere);
int hauteurArbre(T_Arbre abr);
T_Sommet *minimum(T_Arbre abr);

#endif