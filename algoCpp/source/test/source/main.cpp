
#include <iostream>
#include "../include/unit_test.hpp"//c'est pour l'affichage
#include <vector>
#include <map>
#include <fstream>
using namespace std;

static const string FICHIER_CSV="table_fait.csv";

    

int main() {


     string ligne;
    ifstream file (FICHIER_CSV);
    vector<string> V2;
    vector<vector<string>> tableFait;
    getline(file,ligne); //on a pas besoin de la première ligne
    char delim = ','; // Ddefine the delimiter to split by

    if (file.good())
    {
        while (getline(file,ligne,delim))
        {
            V2.push_back(ligne);
            tableFait.push_back(V2);
        }
    }
    else
        cout<<"erreur d'ouverture du fichier"<<endl;

    /*for(auto it = V2.begin(); it != V2.end(); it++) {
          cout << *it<<" ";

       }*/

    for (int i = 0; i < tableFait.size(); i++) {
        for (int j = 0; j < tableFait[i].size(); j++)
            cout << tableFait[i][j] << " ";
        cout << endl;
}



 /*   
 printf("Running tests \n");
    if (testCalculBenefice()) {
        printf("Test réussi! \n");
    }
    else{
        printf("Test failed Calcul Benefice\n");
        return 0;
    }
    if (testRequeteDep()) {
        printf("Test réussi! \n");
    }
    else{
        printf("Test failed  Requete Dep\n");
        return 0;
    }
    if (testCalculBeneficeTotal()) {
        printf("Test réussi! \n");
    }
    else{
        printf("Test failed  CalculBeneficeTotal\n");
        return 0;
    }
    printf("Test réussi! \n");
    return 0;

*/

}
