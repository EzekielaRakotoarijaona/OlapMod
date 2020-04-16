//
//  calculBenefice.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 07/04/2020.
//

#include "../include/calculBenefice.hpp"
#include <time.h>
#include <algorithm>

using namespace std;

int calculBenefice(vector<int> taillesRequetes, vector<int> requetesMaterialisees, int numeroRequete) {
    if(numeroRequete == 0 || numeroRequete >= taillesRequetes.size()) {
        throw "Numero de requete invalide : doit etre supperieur a 0 et dans l'index des requetes possibles";
    }
    int index = requetesMaterialisees.size() - 1;
    int benefice = taillesRequetes[requetesMaterialisees[0]] - taillesRequetes[numeroRequete];
    while( index >= 0) {
        if((numeroRequete & requetesMaterialisees[index]) == numeroRequete || numeroRequete == taillesRequetes.size()-1) {
            if (benefice > taillesRequetes[requetesMaterialisees[index]] - taillesRequetes[numeroRequete]) {
                benefice = taillesRequetes[requetesMaterialisees[index]] - taillesRequetes[numeroRequete];
            }
        }
        index--;
    }
    return benefice;
}

vector<int> requeteDep(int numeroRequete, vector<int> taillesRequetes) {
    vector<int> reponse;
    for(int i = 1; i<taillesRequetes.size(); i++) {
        if((i & numeroRequete) == i || i == taillesRequetes.size()-1){
            reponse.push_back(i);
        }
    }
    return reponse;
}

vector<int> calculBeneficeTotal(vector<int> taillesRequetes, int nombreAMaterialiser) {
    
    double time_spent = 0.0;
    clock_t begin = clock();
    vector<int> requetesMaterialisees;
    requetesMaterialisees.push_back(taillesRequetes.size()-2);
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

int maxBenefice(vector<int> taillesRequetes, vector<int> requetesMaterialisees) {
    int maxBenefice = 0;
    int indexMax = 1;
    printf("\nLes benefice trouves sont : ");
    for(int i = 1; i < taillesRequetes.size(); i++) {
        int beneficeActuel = calculerBeneficeReel(taillesRequetes, requetesMaterialisees, i);
        printf("requete : %d = %d,", i, beneficeActuel);
        if(beneficeActuel > maxBenefice) {
            maxBenefice = beneficeActuel;
            indexMax = i;
        }
    }
    printf("\n");
    return indexMax;
}

int calculerBeneficeReel(vector<int> taillesRequetes, vector<int> requetesMaterialisees, int numeroRequete){
    if(requetesMaterialisees.size() == 1) {
        return calculBenefice(taillesRequetes,requetesMaterialisees, numeroRequete) * requeteDep(numeroRequete, taillesRequetes).size();
    }
    else {
        int benefice = calculBenefice(taillesRequetes,requetesMaterialisees, numeroRequete);
        int maxBenefice = benefice;
        vector<int> requeteDepTab = requeteDep(numeroRequete, taillesRequetes);
        for(int i=0; i< requeteDepTab.size(); i++) {
            if(requeteDepTab[i] != numeroRequete) {
                //afficherVector(requetesMaterialisees);
                int parQuiJeSuisCalculerValeur = parQuiJeSuisCalculer(taillesRequetes, requetesMaterialisees, requeteDepTab[i]);
                //printf("\nJe suis la requete %d et je suis calcule par %d pour la requete %d\n", requeteDepTab[i], parQuiJeSuisCalculerValeur, numeroRequete);
                if(parQuiJeSuisCalculerValeur == parQuiJeSuisCalculer(taillesRequetes, requetesMaterialisees, numeroRequete)) {
                    maxBenefice += benefice;
                } else {
                    if(taillesRequetes[parQuiJeSuisCalculerValeur] > taillesRequetes[numeroRequete]) {
                        //printf("\n Je suis a la requete %d pour la requete %d mon benefice simple est %d - %d = %d \n",i, numeroRequete, taillesRequetes[parQuiJeSuisCalculerValeur], taillesRequetes[numeroRequete], taillesRequetes[parQuiJeSuisCalculerValeur] - taillesRequetes[numeroRequete] );
                        maxBenefice += taillesRequetes[parQuiJeSuisCalculerValeur] - taillesRequetes[numeroRequete];
                    }
                }
            }
        }
        return benefice;
    }
}

int parQuiJeSuisCalculer(vector<int> taillesRequetes, vector<int> requetesMaterialisees, int numeroRequete) {
    if(numeroRequete == 0 || numeroRequete >= taillesRequetes.size()) {
        throw "Numero de requete invalide : doit etre supperieur a 0 et dans l'index des requetes possibles";
    }
    int index = requetesMaterialisees.size() - 1;
    int indexMax = requetesMaterialisees[0];
    for(int i = 0 ; i < requetesMaterialisees.size(); i++) {
        if((numeroRequete & requetesMaterialisees[index]) == numeroRequete || numeroRequete == taillesRequetes.size()-1) {
            if (taillesRequetes[requetesMaterialisees[index]] < taillesRequetes[indexMax]) {
                indexMax = requetesMaterialisees[index];
            }
        }
        index--;
    }
    return indexMax;
}

void afficherVector(vector<int> vector) {
    printf("\nLe vecteur est : ");
    for(int i = 0; i<vector.size() ; i++){
        printf("%d, ", vector[i]);
    }
    printf("\n");
}




