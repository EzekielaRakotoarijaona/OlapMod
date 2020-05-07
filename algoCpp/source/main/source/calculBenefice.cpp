//
//  calculBenefice.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 07/04/2020.
//

#include <iostream>
#include "../include/calculBenefice.hpp"
#include <time.h>
#include <algorithm>
#include <string>
#include <map>
#include <iterator>
#include "../include/nbEnregistrements.hpp"


using namespace std;

static matrice_string tableFaitString_test = {{"USA","T1","TV","5"},
					{"USA","T2","PC","8"},
					{"USA","T1","TV","6"},
					{"USA","T2","PC","4"},
					{"USA","T3","DVD","8"},
					{"MEXICO","T3","DVD","6"},
					{"MEXICO","T1","DVD","2"},
					{"MEXICO","T1","PC","10"},
					{"CANADA","T2","PC","11"},
					{"CANADA","T3","TV","5"} };

int calculBenefice(vector<int> &taillesRequetes, vector<int> &requetesMaterialisees, int numeroRequete) {
    //Calcul de la différence de taille entre la requête courante et la requete matérialisée par laquelle elle est calculée
    if(numeroRequete >= taillesRequetes.size()) {
        throw "Numero de requete invalide : doit etre dans l'index des requetes possibles";
    }
    int index = requetesMaterialisees.size() - 1;
    int benefice = taillesRequetes[requetesMaterialisees[0]] - taillesRequetes[numeroRequete];
    while( index >= 0) {
        //Méthode pour vérifier si l'on peut calculer une requete à partir d'une autre
        if((numeroRequete & requetesMaterialisees[index]) == numeroRequete || numeroRequete == 0) {
            if (benefice > taillesRequetes[requetesMaterialisees[index]] - taillesRequetes[numeroRequete]) {
                benefice = taillesRequetes[requetesMaterialisees[index]] - taillesRequetes[numeroRequete];
            }
        }
        index--;
    }
    return benefice;
}

vector<int> requeteDep(int numeroRequete, vector<int> &taillesRequetes) {
    //Calcul de toutes les dépendances d'une requetes : toutes les requetes calculables à partir de numeroRequete
    vector<int> reponse;
    for(int i = 0; i<=numeroRequete; i++) {
        if((i & numeroRequete) == i){
            reponse.push_back(i);
        }
    }
    return reponse;
}

vector<int> calculBeneficeTotal(vector<int> &taillesRequetes, int nombreAMaterialiser) {
    //Fonction principale à appeler en dehors : dans un main par exemple
    double time_spent = 0.0;
    clock_t begin = clock();
    vector<int> requetesMaterialisees;
    requetesMaterialisees.push_back(taillesRequetes.size()-1);
    //Boucle principale permettant de rajouter la requete ayant le plus de benefice aux requetes matérialisées
    while(requetesMaterialisees.size()-1 != nombreAMaterialiser){
        int size = requetesMaterialisees.size();
        printf("Etape : %d \n", size);
        requetesMaterialisees.push_back(maxBenefice(taillesRequetes,requetesMaterialisees));
    }
 
    afficherVector(requetesMaterialisees);
    clock_t end = clock();
    time_spent += (double)(end - begin)/CLOCKS_PER_SEC;
    printf("Time elpased is %f seconds \n", time_spent);
    return requetesMaterialisees;
}

int maxBenefice(vector<int> &taillesRequetes, vector<int> &requetesMaterialisees) {
    //Détection de la requetes ayant le benefice le plus élevé
    vector<int> benefices;
    benefices.resize(taillesRequetes.size());
    printf("\nLes benefice trouves sont : ");
    //Parcours de chaque requetes pour calculer leur benefice
    #pragma omp parallel for
    for(int i = 0; i < taillesRequetes.size(); i++) {
        int beneficeActuel = calculerBeneficeReel(taillesRequetes, requetesMaterialisees, i);
        benefices[i] = beneficeActuel;
    }
    int indexMax = 0;
    for (int i = 0; i < taillesRequetes.size(); i++) {
        if (benefices[i] > benefices[indexMax]){
            indexMax = i;
        }
    }
    printf("\n");
    return indexMax;
}

int calculerBeneficeReel(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete){
    //Calcul du bénéfice
    
    //A la Première étape, seulement multiplier la différence entre la taille de kla requete materialiee et la taille de la requete courante par le nombre de requete que l'on peut calculer
    if(requetesMaterialisees.size() == 1) {
        return calculBenefice(taillesRequetes,requetesMaterialisees, numeroRequete) * requeteDep(numeroRequete, taillesRequetes).size();
    }
    //Sinon seulement calculer la différence entre la taille de la requete materialiee et la taille de la requete courante,
    else {
        int benefice = calculBenefice(taillesRequetes,requetesMaterialisees, numeroRequete);
        int maxBenefice = benefice;
        //calculer toutes les requetes que l'on peut calculer à partir de numeroRequete
        vector<int> requeteDepTab = requeteDep(numeroRequete, taillesRequetes);
        for(int i=0; i< requeteDepTab.size(); i++) {
            if(requeteDepTab[i] != numeroRequete) {
                int parQuiJeSuisCalculerValeur = parQuiJeSuisCalculer(taillesRequetes, requetesMaterialisees, requeteDepTab[i]);
                //Si la requete est calculée par la même requete que numero requete, additionner benefice
                if(parQuiJeSuisCalculerValeur == parQuiJeSuisCalculer(taillesRequetes, requetesMaterialisees, numeroRequete)) {
                    maxBenefice += benefice;
                } else {
                    //Sinon verifier si la taille de la requete oar laqquele elle est calculée est plus grande que la taille de numeroRequete et si c'est le cas, ajouter au benefice la différence
                    if(taillesRequetes[parQuiJeSuisCalculerValeur] > taillesRequetes[numeroRequete]) {
                        maxBenefice += taillesRequetes[parQuiJeSuisCalculerValeur] - taillesRequetes[numeroRequete];
                    }
                }
            }
        }
        return benefice;
    }
}

int parQuiJeSuisCalculer(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete) {
    if(numeroRequete >= taillesRequetes.size()) {
        throw "Numero de requete invalide : doit etre dans l'index des requetes possibles";
    }
    //Detecter parmi les requetes materialisée, par qui est calculée la requete numéro requete : la requete la plus petite en taille
    int indexMax = requetesMaterialisees[0];
    for(int i = 0 ; i < requetesMaterialisees.size(); i++) {
        if((numeroRequete & requetesMaterialisees[i]) == numeroRequete) {
            if (taillesRequetes[requetesMaterialisees[i]] < taillesRequetes[indexMax]) {
                indexMax = requetesMaterialisees[i];
            }
        }
    }
    return indexMax;
}

void afficherVector(vector<int>& vector) {
    printf("\nLe vecteur est : ");
    for(int i = 0; i<vector.size() ; i++){
        printf("%d, ", vector[i]);
    }
    printf("\n");
}

void afficherTableFait(vector<vector<string>>& tableFait){
    for (int i = 0; i < tableFait.size(); i++) {
        for (int j = 0; j < tableFait[i].size(); j++) {
            cout << tableFait[i][j] << " ";
        }
        cout << endl;
    }
}


void materialiser(vector<vector<string>> & tableFait, int requeteAMaterialiser, int typeOperation, vector<vector<string>> & newTableFait){
    vector<int> entier_Binaire = convertirEnBinaire(requeteAMaterialiser, tableFait[0].size()-1);
    
    vector<vector<string>> tableFaitIntermediaire;
    
    for (int i = 0; i < entier_Binaire.size(); i++) {
        if (entier_Binaire[i] == 1){
            tableFaitIntermediaire.resize(tableFait.size());
            for (int j = 0; j < tableFaitIntermediaire.size(); j++) {
                tableFaitIntermediaire[j].push_back(tableFait[j][i]);
            }
        }
    }
    
    for (int i = 0; i < tableFaitIntermediaire.size(); i++) {
            tableFaitIntermediaire[i].push_back(tableFait[i][tableFait[i].size()-1]);
    }
    
    afficherTableFait(tableFaitIntermediaire);
    
    for (int i = 0; i < tableFaitIntermediaire.size(); i++) {
        int position = findTuple(newTableFait, tableFaitIntermediaire[i]);
        if (position == -1) {
            insertInTableFait(newTableFait, tableFaitIntermediaire[i]);
        }
        else{
            if (typeOperation == 0) {
                int actualValue = stoi(newTableFait[position][newTableFait[position].size()-1]);
                int additionalValue = stoi(tableFaitIntermediaire[i][tableFaitIntermediaire[i].size()-1]);
                int finalValue = actualValue + additionalValue;
                newTableFait[position][newTableFait[position].size()-1] = to_string(finalValue);
            }
            else{
                int actualValue = stoi(newTableFait[position][newTableFait[position].size()-1]);
                int additionalValue = stoi(tableFaitIntermediaire[i][tableFaitIntermediaire[i].size()-1]);
                if (actualValue < additionalValue) {
                    newTableFait[position][newTableFait[position].size()-1] = to_string(additionalValue);
                }
            }
        }
    }
    afficherTableFait(newTableFait);
}

int findTuple(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire){
    if (newTableFait.size() == 0) {
        return -1;
    }
    for (int i = 0; i < newTableFait.size(); i++) {
        int count = 0;
        for (int j = 0; j < tableFaitIntermediaire.size()-1; j++) {
            if (newTableFait[i][j] == tableFaitIntermediaire[j]) {
                count++;
            }
        }
        if (count == tableFaitIntermediaire.size()-1) {
            return i;
        }
    }
    return -1;
}

void insertInTableFait(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire){
    newTableFait.resize(newTableFait.size()+1);
    for (int i = 0; i < tableFaitIntermediaire.size(); i++) {
        newTableFait[newTableFait.size() - 1].push_back(tableFaitIntermediaire[i]);
    }
}


void stockerRequete(vector<int>& vector, matrice_string table_FaitString) {
    matrice_string tableFait_Retournee;
    map<int,matrice_string> map_Sum;
    map<int,matrice_string> map_Max;
    for(int i = 0; i<vector.size() ; i++){
    //map_Sum[vector[i]].push_back(fonctionEZECK(table_FaitString, vector[i],0));
   // map_Max[vector[i]].push_back(fonctionEZECK(table_FaitString, vector[i],1));
   //tableFait_Retournee=tableFaitString_test;
    // map_Sum[vector[i]].push_back(tableFaitString_test);  
    // map_Max[vector[i]].push_back(tableFaitString_test);
    }
/*
for (int i = 0; i < map_Sum.size(); i++) {  // On parcourt la matrice et affiche ses valeurs
        for (int j = 0; j < map_Sum[i].size(); j++)
            cout<< map_Sum[i][j] << " ";
        cout<<" "<<endl;
    }*/
}

//vector<vector<int>> fonctionEZECK( matrice_string  tableFait_Entree, int num_requete, int parametre){

//}





