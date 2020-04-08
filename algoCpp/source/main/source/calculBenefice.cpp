//
//  calculBenefice.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 07/04/2020.
//

#include "../include/calculBenefice.hpp"


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




