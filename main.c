#include "main.h"

int test() {
    T_Arbre abr = creerSommet(15);
    for (int i = 0 ; i<15 ; i++) {
        insererElement(abr, 2*i);
    }
    afficherSommet(abr);
    printf("\n");
    afficherElement(abr);
    return 0;
}

int main(int argv, char* argc[]) {
    // return test();
    return graphicalMain(argv, argc);
}