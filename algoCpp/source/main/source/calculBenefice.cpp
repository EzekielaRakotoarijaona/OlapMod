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
#include <unordered_map> 
#include <iterator>
#include "../include/nbEnregistrements.hpp"
#include <fstream>


using namespace std;

long calculBenefice(vector<long> &taillesRequetes, vector<long> &requetesMaterialisees, long numeroRequete) {
    //Calcul de la différence de taille entre la requête courante et la requete matérialisée par laquelle elle est calculée
    if(numeroRequete >= taillesRequetes.size()) {
        throw "Numero de requete invalide : doit etre dans l'index des requetes possibles";
    }
    long index = requetesMaterialisees.size() - 1;
    long benefice = taillesRequetes[requetesMaterialisees[0]] - taillesRequetes[numeroRequete];
    while( index >= 0) {
        //Méthode pour vérifier si l'on peut calculer une requete à partir d'une autre
        if((numeroRequete & requetesMaterialisees[index]) == numeroRequete) {
            if (benefice > taillesRequetes[requetesMaterialisees[index]] - taillesRequetes[numeroRequete]) {
                benefice = taillesRequetes[requetesMaterialisees[index]] - taillesRequetes[numeroRequete];
            }
        }
        index--;
    }
    return benefice;
}

vector<long> requeteDep(long numeroRequete, vector<long> &taillesRequetes) {
    //Calcul de toutes les dépendances d'une requetes : toutes les requetes calculables à partir de numeroRequete
    vector<long> reponse;
    for(auto i = 0; i<=numeroRequete; i++) {
        if((i & numeroRequete) == i){
            reponse.push_back(i);
        }
    }
    return reponse;
}

vector<long> calculBeneficeTotal(vector<long> &taillesRequetes, long nombreAMaterialiser) {
    //Fonction principale à appeler en dehors : dans un main par exemple
    double time_spent = 0.0;
    clock_t begin = clock();
    vector<long> requetesMaterialisees;
    requetesMaterialisees.push_back(taillesRequetes.size()-1);
    //Boucle principale permettant de rajouter la requete ayant le plus de benefice aux requetes matérialisées
    while(requetesMaterialisees.size()-1 != nombreAMaterialiser){
        long size = requetesMaterialisees.size();
        printf("Etape : %d \n", size);
        long indexMax = maxBenefice(taillesRequetes,requetesMaterialisees);
        if(vectorContains(indexMax, requetesMaterialisees)){
            break;
        }
        requetesMaterialisees.push_back(indexMax);
    }
 
    afficherVector(requetesMaterialisees);
    cout<< "La taille du vecteur est : " << requetesMaterialisees.size() << endl;
    clock_t end = clock();
    time_spent += (double)(end - begin)/CLOCKS_PER_SEC;
    printf("Time elpased is %f seconds \n", time_spent);
    return requetesMaterialisees;
}

long maxBenefice(vector<long> &taillesRequetes, vector<long> &requetesMaterialisees) {
    //Détection de la requetes ayant le benefice le plus élevé
    vector<long> benefices(taillesRequetes.size(), 0);
    printf("\nLes benefice trouves sont : ");
    //Parcours de chaque requetes pour calculer leur benefice
    #pragma omp parallel for
    for(long i = 0; i < taillesRequetes.size(); i++) {
        if(!vectorContains(i, requetesMaterialisees)) {
           long beneficeActuel = calculerBeneficeReel(taillesRequetes, requetesMaterialisees, i);
           benefices[i] = beneficeActuel;
        }
    }
    long indexMax = 0;
    for (long i = 0; i < taillesRequetes.size(); i++) {
        if (benefices[i] > benefices[indexMax]){
            indexMax = i;
        }
    }
    printf("\n");
    return indexMax;
}

bool vectorContains(long value, vector<long> vector){
    //cout << "value " << value;
    //afficherVector(vector);
    for(auto val: vector){
        if(value == val){
            return true;
        }
    }
    return false;
}

long calculerBeneficeReel(vector<long>& taillesRequetes, vector<long>& requetesMaterialisees, long numeroRequete){
    //Calcul du bénéfice
    
    //A la Première étape, seulement multiplier la différence entre la taille de kla requete materialiee et la taille de la requete courante par le nombre de requete que l'on peut calculer
    if(requetesMaterialisees.size() == 1) {
        return calculBenefice(taillesRequetes,requetesMaterialisees, numeroRequete) * requeteDep(numeroRequete, taillesRequetes).size();
    }
    //Sinon seulement calculer la différence entre la taille de la requete materialiee et la taille de la requete courante,
    else {
        long benefice = calculBenefice(taillesRequetes,requetesMaterialisees, numeroRequete);
        long maxBenefice = benefice;
        //calculer toutes les requetes que l'on peut calculer à partir de numeroRequete
        vector<long> requeteDepTab = requeteDep(numeroRequete, taillesRequetes);
        for(long i=0; i< requeteDepTab.size(); i++) {
            if(requeteDepTab[i] != numeroRequete) {
                long parQuiJeSuisCalculerValeur = parQuiJeSuisCalculer(taillesRequetes, requetesMaterialisees, requeteDepTab[i]);
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

long parQuiJeSuisCalculer(vector<long>& taillesRequetes, vector<long>& requetesMaterialisees, long numeroRequete) {
    if(numeroRequete >= taillesRequetes.size()) {
        throw "Numero de requete invalide : doit etre dans l'index des requetes possibles";
    }
    //Detecter parmi les requetes materialisée, par qui est calculée la requete numéro requete : la requete la plus petite en taille
    long indexMax = requetesMaterialisees[0];
    for(long i = 0 ; i < requetesMaterialisees.size(); i++) {
        if((numeroRequete & requetesMaterialisees[i]) == numeroRequete) {
            if (taillesRequetes[requetesMaterialisees[i]] < taillesRequetes[indexMax]) {
                indexMax = requetesMaterialisees[i];
            }
        }
    }
    return indexMax;
}

void afficherVector(vector<long>& vector) {
    printf("\nLe vecteur est : ");
    for(long i = 0; i<vector.size() ; i++){
        printf("%d, ", vector[i]);
    }
    printf("\n");
}

void afficherTableFait(vector<vector<string>>& tableFait){
    for (long i = 0; i < tableFait.size(); i++) {
        for (long j = 0; j < tableFait[i].size(); j++) {
            cout << tableFait[i][j] << " ";
        }
        cout << endl;
    }
}


void materialiser(vector<vector<string>> & tableFait, long requeteAMaterialiser, long typeOperation, vector<vector<string>> & newTableFait){
    //Si la requete est 0 il suffit d'additioner ou maximiser le tout
    newTableFait.resize(0);
    if(requeteAMaterialiser == 0){
        newTableFait.resize(2);
        newTableFait[0].push_back(tableFait[0][tableFait[0].size()-1]);
        newTableFait[1].push_back(tableFait[1][tableFait[1].size()-1]);
        for(long i = 2; i < tableFait.size(); i++){
            if(typeOperation == 0){
                addition(tableFait, newTableFait, i, 1);
            }
            else {
                max(tableFait, newTableFait, i, 1);
            }
        }
        return;
    }
    //Sinon
    vector<long> entier_Binaire = convertirEnBinaire(requeteAMaterialiser, tableFait[0].size()-1);
    vector<vector<string>> tableFaitIntermediaire;
    //Création  d'une table de fait intermédiaire récupérant les colonnes concernées
    for (long i = 0; i < entier_Binaire.size(); i++) {
        if (entier_Binaire[i] == 1){
            tableFaitIntermediaire.resize(tableFait.size());
            for (long j = 0; j < tableFaitIntermediaire.size(); j++) {
                tableFaitIntermediaire[j].push_back(tableFait[j][i]);
            }
        }
    }
    //On y ajoute les faits
    for (long i = 0; i < tableFaitIntermediaire.size(); i++) {
            tableFaitIntermediaire[i].push_back(tableFait[i][tableFait[i].size()-1]);
    }
    //On stock dans la vraie table de fait les tuples de manière unique et de sorte à appliquer la fonction d'aggrégation
    for (long i = 0; i < tableFaitIntermediaire.size(); i++) {
        //Position renvoie la position du tuple dans la nnouvelleTableDeFAit
        long position = findTuple(newTableFait, tableFaitIntermediaire[i]);
        //Si la position est à -1 on insère le tuple dans la table de fait
        if (position == -1) {
            insertInTableFait(newTableFait, tableFaitIntermediaire[i]);
        }
        else{
            //SI 0 calcul de la somme
            if (typeOperation == 0) {
                addition(tableFaitIntermediaire, newTableFait, i, position);
            }
            //Si 1 Calcul du Max
            else{
                max(tableFaitIntermediaire, newTableFait, i, position);
            }
        }
    }
    tableFaitIntermediaire.clear();
}

long findTuple(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire){
    //Si la table de fait est de taille 0 alors il est sûr que le tuple n'est pas encore dans la table
    if (newTableFait.size() == 0) {
        return -1;
    }
    //Sinon on parcours par colonne pour voir s'il y a une colonne qui correspond exactement les mêmes tuples
    for (long i = 0; i < newTableFait.size(); i++) {
        long count = 0;
        for (long j = 0; j < tableFaitIntermediaire.size()-1; j++) {
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
    for (long i = 0; i < tableFaitIntermediaire.size(); i++) {
        newTableFait[newTableFait.size() - 1].push_back(tableFaitIntermediaire[i]);
    }
}


void stockerRequete(vector<long>& requetesMaterialisees, vector<vector<string>>& table_FaitString, unordered_map<long,vector<vector<string>>>& sum, unordered_map<long,vector<vector<string>>>& max) {
    
    sum.clear();
    max.clear();
    
    #pragma omp parallel for
    for(long i = 1; i<requetesMaterialisees.size() ; i++){
        vector<vector<string>> newTableFaitSomme;
        vector<vector<string>> newTableFaitMax;
        materialiser(table_FaitString, requetesMaterialisees[i],0, newTableFaitSomme);
        materialiser(table_FaitString, requetesMaterialisees[i],1, newTableFaitMax);
        sum[requetesMaterialisees[i]] = newTableFaitSomme;
        max[requetesMaterialisees[i]] = newTableFaitMax;
    }
}

void addition(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait){
    long actualValue = stol(newTableFait[positionNewTableFait][newTableFait[positionNewTableFait].size()-1]);
    long additionalValue = stol(tableFait[positionTableFait][tableFait[positionTableFait].size()-1]);
    long finalValue = actualValue + additionalValue;
    newTableFait[positionNewTableFait][newTableFait[positionNewTableFait].size()-1] = to_string(finalValue);
}

void max(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait){
    long actualValue = stol(newTableFait[positionNewTableFait][newTableFait[positionNewTableFait].size()-1]);
    long additionalValue = stol(tableFait[positionTableFait][tableFait[positionTableFait].size()-1]);
    if (actualValue < additionalValue) {
        newTableFait[positionNewTableFait][newTableFait[positionNewTableFait].size()-1] = to_string(additionalValue);
    }
}

void materialiserRequete(vector<string>& numeroDeRequete,vector<long>& taillesRequetes, vector<long>& requetesMaterialisees, vector<vector<string>>& table_FaitString, unordered_map<long,vector<vector<string>>>& unordered_map_Sum, unordered_map<long,vector<vector<string>>>& unordered_map_Max, long typeOperation,  vector<vector<string>>& newTableFait) {
    //On vide la table de fait si elle a déjà été utilisé auparavant
    newTableFait.clear();
    //On recupère l'entier correspondant à la requete
    vector<long> binaryRequest = fromStringToBinary(numeroDeRequete, table_FaitString);
    long requete = conversionBinaireAEntier(binaryRequest);
    //Si la requete est déjà matérialisée on renvoie depuis la map ou la table de fait de base
    if (find(requetesMaterialisees.begin(), requetesMaterialisees.end(), requete) != requetesMaterialisees.end()){
        if(requete == requetesMaterialisees[0]) {
            cout << "Calcul sur table fait actuel" << endl;
            newTableFait = table_FaitString;
        }
        else {
            if(typeOperation == 0) {
                cout << "Recuperation somme depuis map" << endl;
                newTableFait = unordered_map_Sum[requete];
            }
            else{
                cout << "Recuperation max depuis map" << endl;
                newTableFait = unordered_map_Max[requete];
            }
        }
    }
    //Sinon on calcul la table à partir de la requete deja materialisée la plus proche 
    else {
        long parQuiJesuiCalculer = parQuiJeSuisCalculer(taillesRequetes, requetesMaterialisees, requete);
        if(parQuiJesuiCalculer == requetesMaterialisees[0]) {
            cout << "Materialisation a partir de la table de fait de base" << endl;
            materialiser(table_FaitString, requete, typeOperation, newTableFait);
        }
        else if(typeOperation == 0) {
            cout << "Materialisation somme depuis map" << endl;
            binaryRequest = fromStringToBinary(numeroDeRequete, unordered_map_Sum[parQuiJesuiCalculer]);
            requete = conversionBinaireAEntier(binaryRequest);
            materialiser(unordered_map_Sum[parQuiJesuiCalculer], requete, typeOperation, newTableFait);
        }
        else {
            cout << "Materialisation max depuis map" << endl;
            binaryRequest = fromStringToBinary(numeroDeRequete, unordered_map_Sum[parQuiJesuiCalculer]);
            requete = conversionBinaireAEntier(binaryRequest);
            materialiser(unordered_map_Max[parQuiJesuiCalculer], requete, typeOperation, newTableFait);
        }
    }
}

long conversionBinaireAEntier(vector<long>& numeroDeRequete) {
    string binaireNumeroRequete;
    for (long i = 0; i < numeroDeRequete.size(); i++) {
        binaireNumeroRequete += to_string(numeroDeRequete[i]);
    }
    return stol(binaireNumeroRequete, 0, 2);
}

vector<long> fromStringToBinary(vector<string> stringRequest, vector<vector<string>> &tableFaitString) {
    vector<long> binaryResult(tableFaitString[0].size()-1, 0);
    for(auto i=0; i<binaryResult.size(); i++) {
        if(std::find(stringRequest.begin(), stringRequest.end(), tableFaitString[0][i]) != stringRequest.end()) {
            binaryResult[i] = 1;
        }
    }
    return binaryResult;
}

long espaceMemoireUtilise(vector<long> &taillesRequetes, vector<long>& requetesMaterialisees){
    long espaceMemoire = 0;
    for(long i = 1; i<requetesMaterialisees.size(); i++) {
        espaceMemoire += taillesRequetes[requetesMaterialisees[i]];
    }
    return espaceMemoire;
}

long espaceMemoirePrevu(vector<long> &taillesRequetes, long nbPrevu){
    long espaceMemoire = 0;
    vector<long> copieTailles = taillesRequetes;
    sort(copieTailles.begin(), copieTailles.end(), greater<long>());
    for(long i = 1; i<nbPrevu; i++){
        espaceMemoire += copieTailles[i];
    }
    return espaceMemoire;
}

void exportFichier(vector<vector<string>> tableFait, string nomFichier){
    ofstream myfile(nomFichier);
    
    for (long n=0; n < tableFait.size(); n++)
    {
        for (long i = 0; i < tableFait[n].size(); i++) {
            if (i == tableFait[n].size() - 1) {
                myfile << tableFait[n][i];
            }
            else
            myfile << tableFait[n][i] << ",";
        }
        myfile << endl;
    }
    myfile.close();
}


