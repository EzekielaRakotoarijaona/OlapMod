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
        if((i & numeroRequete) == i || i == 0){
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
    int maxBenefice = 0;
    int indexMax = 1;
    printf("\nLes benefice trouves sont : ");
    //Parcours de chaque requetes pour calculer leur benefice
    for(int i = 0; i < taillesRequetes.size(); i++) {
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
        if((numeroRequete & requetesMaterialisees[i]) == numeroRequete || numeroRequete == 0) {
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




