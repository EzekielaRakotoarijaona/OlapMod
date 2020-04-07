
#include <iostream>//c'est pour l'affichage
#include <vector>
#include<set>
#include <ctime>//ça sert pour la génération aléatoire
#include <cmath>//ça sert pour la fonction pow (puissance)
#include "../include/nbEnregistrements.hpp"
#include "../include/calculBenefice.hpp"


int main() {
    int l = 100000; //100000 lignes
    int c = 5; // 5 colonnes
    matrice M;
    srand(time(NULL));//c'est pour qu'à chaque exécution, on ait des nombres différents
    generer(M, l, c);
    //pour tester la fonction taille
    vector<int> W(3); W[0] = 1;  W[1] = 3; W[2] = 4;// je teste sur les colonnes 1, 3 et 4
    cout << "Dans la matrice M, en considérant les 3 colonnes 1, 2 et 3, le nombre de triplets distincts est " << taille(M, W) << endl;
    cout << "_____________________________________" << endl;
    //maintenant, je calcule toutes les tailles
    toutes_les_tailles(M);
    //la fonction toutes_les_tailles calcule et affiche toutes les tailles. Noter qu'en réalité, on n'a pas besoin d'afficher.
    return 0;
}
