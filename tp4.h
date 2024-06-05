#ifndef TP4_HEAD
#define TP4_HEAD

typedef struct Sommet T_Sommet;
typedef struct Classique Classique;
typedef T_Sommet *T_Arbre;

struct Sommet {
    int borneInf;
    int borneSup;
    struct Sommet *filsGauche;
    struct Sommet *filsDroit;
};

struct Classique {
    int valeur;
    struct AutreSommet *filsGauche;
    struct AutreSommet *filsDroit;
};

T_Sommet *creerSommet(int element);
T_Arbre insererElement(T_Arbre abr, int element);
T_Sommet *rechercherElement(T_Arbre abr, int element);
void afficherSommet(T_Arbre abr);
void afficherElement(T_Arbre abr);
T_Arbre supprimerElement(T_Arbre abr, int element);
unsigned long tailleMemoire(T_Arbre abr, unsigned long *tailleReel, unsigned long *tailleClassique);

T_Sommet *rechercherElementAvecPere(T_Arbre abr, int element, T_Sommet **pere);
void supprimerNoeud(T_Sommet *cible, T_Sommet *pere);
void separerIntervalle(T_Sommet *cible, int element);
void fusionnerSommet(T_Arbre abr, T_Sommet *cible, int element);
int hauteurArbre(T_Arbre abr);
T_Sommet *minimumAvecPere(T_Arbre abr, T_Sommet **pere);
int nombreNoeuds(T_Arbre abr);
int nombreElements(T_Arbre abr);
void libererArbre(T_Arbre abr);

#endif