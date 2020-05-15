
#include <iostream>//c'est pour l'affichage
#include <vector>
#include <set>
#include <ctime>//ça sert pour la génération aléatoire
#include <cmath>//ça sert pour la fonction pow (puissance)
#include "../include/nbEnregistrements.hpp"
#include "../include/calculBenefice.hpp"
#include <unordered_map>

int main(int argc, char *argv[]) {
    if(argc > 1) {
        //SI on lance le main avec une le chemin d'un fichier csv en parametre
        vector<vector<string>> tableFaitString = chargerFichiers(argv[1]);
        matrice tableFait = conversion(tableFaitString);
        //Calcul et affichage des tailles de la table de fait
        vector<int> taillesRequetes = toutes_les_tailles(tableFait);
        cout << "Combien de requêtes à matérialiser ? : ";
        int nombreAMaterialiser = 0;
        //Demande du nombre de requete à matérialiser en plus de la table de fait
        cin >> nombreAMaterialiser;
        while(nombreAMaterialiser < 0 || nombreAMaterialiser > taillesRequetes.size()-1){
            cout << "erreur de saisie, retapez" << endl;
            cin.clear();
            cin.ignore(999, '\n');
            cin >> nombreAMaterialiser;
        }
        //Calcul des bénéfices
        vector<int> requeteMaterialiser = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
        unordered_map<int,vector<vector<string>>> unordered_map_Sum;
        unordered_map<int,vector<vector<string>>> unordered_map_Max;
        stockerRequete(requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max);
        cout << "Quelle requete stockée afficher? : " << endl;
        int requeteAMaterialiser = 0;
        cin >> requeteAMaterialiser;
        while(requeteAMaterialiser < 0 || requeteAMaterialiser > taillesRequetes.size()-1){
            cout << "erreur de saisie, retapez" << endl;
            cin.clear();
            cin.ignore(999, '\n');
            cin >> requeteAMaterialiser;
        }
        afficherTableFait(unordered_map_Max[requeteAMaterialiser]);
        return 0;
    }
    //Cas si le main est lancé sans paramètre : lancement avec une table de fait aléatoire
    int l = 100000; //100000 lignes
    int c = 5; // 5 colonnes
    matrice M;
    srand(time(NULL));//c'est pour qu'à chaque exécution, on ait des nombres différents
    cout << "Generation d'une table aléatoire de taille 100000" << endl;
    generer(M, l, c);
    cout << "Début du calcul des tailles" << endl;
    //maintenant, je calcule toutes les tailles
    vector<int> taillesRequetes = toutes_les_tailles(M);
    int nombreAMaterialiser = 4;
    // calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    //la fonction toutes_les_tailles calcule et affiche toutes les tailles. Noter qu'en réalité, on n'a pas besoin d'afficher.
    
    vector<vector<string>> newTableFait;
    vector<vector<string>> tableFaitString = chargerFichiers("../resources/table_fait_string.csv");
    materialiser(tableFaitString, 0, 1, newTableFait);
    afficherTableFait(newTableFait);
    
    
    return 0;
}


