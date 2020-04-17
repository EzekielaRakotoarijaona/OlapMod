
#include <iostream>//c'est pour l'affichage
#include <vector>
#include<set>
#include <ctime>//ça sert pour la génération aléatoire
#include <cmath>//ça sert pour la fonction pow (puissance)
#include "../include/nbEnregistrements.hpp"
#include "../include/calculBenefice.hpp"


int main(int argc, char *argv[]) {
    if(argc > 1) {
        matrice tableFait = chargerFichiers(argv[1]);
        vector<int> taillesRequetes = toutes_les_tailles(tableFait);
        cout << "Combien de requêtes à matérialiser ? : ";
        int nombreAMaterialiser = 0;
        cin >> nombreAMaterialiser;
        while(nombreAMaterialiser < 1 || nombreAMaterialiser > taillesRequetes.size()-1){
            cout << "erreur de saisie, retapez" << endl;
            cin.clear();
            cin.ignore(999, '\n');
            cin >> nombreAMaterialiser;
        }
        calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
        return 0;
    }
    int l = 100000; //100000 lignes
    int c = 3; // 5 colonnes
    matrice M;
    srand(time(NULL));//c'est pour qu'à chaque exécution, on ait des nombres différents
    cout << "Generation d'une table aléatoire de taille 100000" << endl;
    generer(M, l, c);
    cout << "Début du calcul des tailles" << endl;
    //maintenant, je calcule toutes les tailles
    vector<int> taillesRequetes = toutes_les_tailles(M);
    int nombreAMaterialiser = 4;
    calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    //la fonction toutes_les_tailles calcule et affiche toutes les tailles. Noter qu'en réalité, on n'a pas besoin d'afficher.
    return 0;
}
