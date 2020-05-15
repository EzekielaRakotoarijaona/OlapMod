#include "../include/unit_test.hpp"
#include "../../main/include/calculBenefice.hpp"
#include "../../main/include/nbEnregistrements.hpp"
#include <iostream>
#include <stdio.h>
#include <unordered_map>

bool testCalculBenefice(){
    
    int resultatAttendu = 3;
    int resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 2);
    
    //Test 1 : Dans le cas où le résultatAttendu est correcte
    if (resultatReel != resultatAttendu) {
        printf("Test failed : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
        return false;
    }
    printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
    
    //Test 2 : Dans le cas où le résultatAttendu est faux
    resultatAttendu = 3;
    resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 2);
    
    if (resultatReel == resultatAttendu) {
        printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
        
        //Test 3 : On rajoute un noeud matérialisé de plus : noeud 3
        requetesMaterialisees.push_back(3);
        resultatAttendu = 1;
        resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 2);
        if(resultatAttendu == resultatReel) {
            printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
            
            //Test 4 : On rajoute une autre dimension non calacuble à partir du numeroRequete
            requetesMaterialisees.push_back(1);
            resultatAttendu = 1;
            resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 2);
            if(resultatAttendu == resultatReel) {
                printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
                //Test 5: Ajout d'une dimension moins benéfique que celles précédentes
                    requetesMaterialisees.push_back(6);
                    resultatAttendu = 1;
                    resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 2);
                    if(resultatAttendu == resultatReel) {
                        printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
                        //Test 6: Changement du numero de la reqiete en 8
                        requetesMaterialisees.push_back(6);
                        resultatAttendu = 2;
                        resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 0);
                        if(resultatAttendu == resultatReel) {
                            printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
                            return true;
                        }
                    }
                }
            }
    }
    printf("Test failed : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
    return false;
}


bool testRequeteDep(){
    vector<int> resultatAttendu = {0,1,2,3,4,5,6,7};
    vector<int> resultatReel = requeteDep(7, taillesRequetes);
    if(resultatAttendu != resultatReel){
        printf("Test Failed\n");
        return false;
    }
    resultatAttendu = {0,2,4,6};
    resultatReel = requeteDep(6, taillesRequetes);
    if(resultatAttendu != resultatReel){
        printf("Test Failed \n");
        return false;
    }
    resultatAttendu = {0};
    resultatReel = requeteDep(0, taillesRequetes);
    if(resultatAttendu != resultatReel){
        printf("Test Failed \n");
        return false;
    }
    return true;
}

bool testCalculBeneficeTotal() {
    //Test 1
    int nombreAMaterialiser = 1;
    vector<int> resultatAttendu = {7,1};
    vector<int> resultatReel = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    if(resultatAttendu != resultatReel){
        printf("Test Failed Test 1 Calcul Benefice Total \n");
        return false;
    }
    
    //Test 2
    nombreAMaterialiser = 2;
    resultatAttendu = {7,1,2};
    resultatReel = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    if(resultatAttendu != resultatReel){
        printf("Test Failed Test 2 Calcul Benefice Total \n");
        return false;
    }
    
    //Test 3
    nombreAMaterialiser = 2;
    resultatAttendu = {7,5,2};
    resultatReel = calculBeneficeTotal(taillesRequetes2, nombreAMaterialiser);
    if(resultatAttendu != resultatReel){
        printf("Test Failed Test 3 Calcul Benefice Total \n");
        return false;
    }
    
    return true;
}


bool testChargerFichier(string filePath) {
    matrice resultatReel = conversion(chargerFichiers(filePath));
    if(tableFait.size() != resultatReel.size()){
        printf("Test Failed test Charger fichier 1 \n");
        return false;
    }
    for (int i = 0; i < tableFait.size(); i++) {
        if(tableFait[i].size() != resultatReel[i].size()){
            printf("Test Failed test Charger fichier 2 \n");
            return false;
        }
        for (int j = 0; j < tableFait[i].size(); j++){
            if(tableFait[i][j] != resultatReel[i][j]){
                printf("Test Failed test Charger fichier 3 \n");
                return false;
            }
        }
    }
    return true;
}

bool testTaillesRequetes(){
 vector<int> resultatReel = toutes_les_tailles(tableFait);
 if(resultatAttenduTestTaillesRequetes != resultatReel){
    printf("Test Failed test Tailles Requetes \n");
    return false;
}
return true;
}

bool testIntegrationCalculBeneficeWorkflow(string filePath) {
    //Lancement d'un test d'integration
    vector<vector<string>> tableFaitString = chargerFichiers(filePath);
    matrice tableFait = conversion(tableFaitString);
    //Calcul et affichage des tailles de la table de fait
    vector<int> taillesRequetes = toutes_les_tailles(tableFait);
    int nombreAMaterialiser = 3;
    //Calcul des bénéfices
    vector<int> resultatAttendu = {7,1,2,4};
    vector<int> resultatReel = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    if(resultatAttendu != resultatReel){
        printf("Integration Test Benefice WorkFlow Failed \n");
        return false;
    }
    return true;
}


bool testMaterialiser(string filePath){
    
    vector<vector<string>> newTableFaitSomme;
    vector<vector<string>> newTableFaitMax;
    vector<vector<string>> tableFaitString = chargerFichiers("../resources/table_fait_string_test.csv");
    
    materialiser(tableFaitString, 3, 0, newTableFaitSomme);
    materialiser(tableFaitString, 3, 1, newTableFaitMax);
 
    if(newTableFaitSomme.size() != tableFaitAttenduSomme.size()){
        printf("Test Failed test 1 table fait SOMME \n");
        return false;
    }
    for (int i = 0; i < newTableFaitSomme.size(); i++) {
        if(newTableFaitSomme[i].size() != tableFaitAttenduSomme[i].size()){
            printf("Test Failed test 2 table fait SOMME \n");
            return false;
        }
        for (int j = 0; j < newTableFaitSomme[i].size(); j++){
            if(newTableFaitSomme[i][j] != tableFaitAttenduSomme[i][j]){
                printf("Test Failed test 3 table fait SOMME \n");
                return false;
            }
        }
    }
    
    if(newTableFaitMax.size() != tableFaitAttenduMax.size()){
        printf("Test Failed test 1 table fait MAX \n");
        return false;
    }
    for (int i = 0; i < newTableFaitMax.size(); i++) {
        if(newTableFaitMax[i].size() != tableFaitAttenduMax[i].size()){
            printf("Test Failed test 2 table fait MAX \n");
            return false;
        }
        for (int j = 0; j < newTableFaitMax[i].size(); j++){
            if(newTableFaitMax[i][j] != tableFaitAttenduMax[i][j]){
                printf("Test Failed test 3 table fait MAX \n");
                return false;
            }
        }
    }
    printf("Test réussi SOMME et MAX \n");
    return true;
}


bool testStocker(string filePath) {
    vector<vector<string>> tableFaitString = chargerFichiers(filePath);
    matrice tableFait = conversion(tableFaitString);
    //Calcul et affichage des tailles de la table de fait
    vector<int> taillesRequetes = toutes_les_tailles(tableFait);
    int nombreAMaterialiser = 6;
    //Demande du nombre de requete à matérialiser en plus de la table de fait
    //Calcul des bénéfices
    vector<int> requeteMaterialiser = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    unordered_map<int,vector<vector<string>>> unordered_map_Sum;
    unordered_map<int,vector<vector<string>>> unordered_map_Max;
    stockerRequete(requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max);
    afficherTableFait(unordered_map_Sum[3]);
    if(unordered_map_Sum[3].size() != tableFaitAttenduSomme.size()){
           printf("Test Failed test 1 MAP SOMME \n");
           return false;
       }
       for (int i = 0; i < unordered_map_Sum[3].size(); i++) {
           if(unordered_map_Sum[3][i].size() != tableFaitAttenduSomme[i].size()){
               printf("Test Failed test 2 MAP SOMME \n");
               return false;
           }
           for (int j = 0; j < unordered_map_Sum[3][i].size(); j++){
               if(unordered_map_Sum[3][i][j] != tableFaitAttenduSomme[i][j]){
                   printf("Test Failed test 3 MAP SOMME \n");
                   return false;
               }
           }
       }
       
       if(unordered_map_Max[3].size() != tableFaitAttenduMax.size()){
           printf("Test Failed test 1 MAP MAX \n");
           return false;
       }
       for (int i = 0; i < unordered_map_Max[3].size(); i++) {
           if(unordered_map_Max[3][i].size() != tableFaitAttenduMax[i].size()){
               printf("Test Failed test 2 MAP MAX \n");
               return false;
           }
           for (int j = 0; j < unordered_map_Max[3][i].size(); j++){
               if(unordered_map_Max[3][i][j] != tableFaitAttenduMax[i][j]){
                   printf("Test Failed test 3 MAP MAX \n");
                   return false;
               }
           }
       }
       printf("Test réussi Stocker \n");
       return true;
}

bool testConversionBinaireEntier() {
    //Test 1
    if(conversionBinaireAEntier(entierBinaire1) != 1){
        printf("Test Failed Test 1 Conversion Binaire A Entier \n");
        return false;
    }
    
    //Test 2
    if(conversionBinaireAEntier(entierBinaire2) != 18){
        printf("Test Failed Test 2 Conversion Binaire A Entier  \n");
        return false;
    }
    
    //Test 3
    if(conversionBinaireAEntier(entierBinaire3) != 30){
        printf("Test Failed Test 3 Conversion Binaire A Entier  \n");
        return false;
    }
    
    //Test 4
    if(conversionBinaireAEntier(entierBinaire4) != 31){
        printf("Test Failed Test 4 Conversion Binaire A Entier  \n");
        return false;
    }
    printf("Test réussi Conversion Binaire A Entier \n");
    return true;
}

bool testMateriliserRequete(string filePath) {
    vector<vector<string>> tableFaitString = chargerFichiers(filePath);
    matrice tableFait = conversion(tableFaitString);
    //Calcul et affichage des tailles de la table de fait
    vector<int> taillesRequetes = toutes_les_tailles(tableFait);
    int nombreAMaterialiser = 6;
    //Demande du nombre de requete à matérialiser en plus de la table de fait
    //Calcul des bénéfices
    vector<int> requeteMaterialiser = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    unordered_map<int,vector<vector<string>>> unordered_map_Sum;
    unordered_map<int,vector<vector<string>>> unordered_map_Max;
    stockerRequete(requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max);
    vector<vector<string>> newTableFait;
    
    //Test 1 Materialisation provenant de la map
    materialiserRequete(entierBinaireTestMaterialiserRequete,taillesRequetes, requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max, 0, newTableFait);
    if(newTableFait.size() != tableFaitAttenduSomme.size()){
        printf("Test Failed test 1 Materialiser of map sum \n");
        return false;
    }
    for (int i = 0; i < newTableFait.size(); i++) {
        if(newTableFait[i].size() != tableFaitAttenduSomme[i].size()){
            printf("Test Failed test 2 Materialiser of map sum \n");
            return false;
        }
        for (int j = 0; j < newTableFait[i].size(); j++){
            if(newTableFait[i][j] != tableFaitAttenduSomme[i][j]){
                printf("Test Failed test 3 Materialiser of map sum \n");
                return false;
            }
        }
    }
    
    //Test 2 Materialisation provenant de la table de fait de base
    materialiserRequete(entierBinaireTestMaterialiserRequete2,taillesRequetes, requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max, 0, newTableFait);
    if(newTableFait.size() != tableFaitAttenduSommeMaterialiser.size()){
        printf("Test Failed test 1 Materialiser from extern sum \n");
        return false;
    }
    for (int i = 0; i < newTableFait.size(); i++) {
        if(newTableFait[i].size() != tableFaitAttenduSommeMaterialiser[i].size()){
            printf("Test Failed test 2 Materialiser from extern sum\n");
            return false;
        }
        for (int j = 0; j < newTableFait[i].size(); j++){
            if(newTableFait[i][j] != tableFaitAttenduSommeMaterialiser[i][j]){
                printf("Test Failed test 3 Materialiser from extern sum\n");
                return false;
            }
        }
    }
    
    //Test 3 Materialisation provenant de la table de fait de base
    materialiserRequete(entierBinaireTestMaterialiserRequete2,taillesRequetes, requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max, 1, newTableFait);
    if(newTableFait.size() != tableFaitAttenduMaxMaterialiser.size()){
        printf("Test Failed test 1 Materialiser from extern max \n");
        return false;
    }
    for (int i = 0; i < newTableFait.size(); i++) {
        if(newTableFait[i].size() != tableFaitAttenduMaxMaterialiser[i].size()){
            printf("Test Failed test 2 Materialiser from extern max\n");
            return false;
        }
        for (int j = 0; j < newTableFait[i].size(); j++){
            if(newTableFait[i][j] != tableFaitAttenduMaxMaterialiser[i][j]){
                printf("Test Failed test 3 Materialiser from extern max\n");
                return false;
            }
        }
    }
    
    //Test 4 Materialisation de la table de fait de base
    materialiserRequete(entierBinaireTestMaterialiserRequete3,taillesRequetes, requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max, 1, newTableFait);
    if(newTableFait.size() != tableFaitString_test.size()){
        printf("Test Failed test 1 Materialiser from real table \n");
        return false;
    }
    for (int i = 0; i < newTableFait.size(); i++) {
        if(newTableFait[i].size() != tableFaitString_test[i].size()){
            printf("Test Failed test 2 Materialiser from real table\n");
            return false;
        }
        for (int j = 0; j < newTableFait[i].size(); j++){
            if(newTableFait[i][j] != tableFaitString_test[i][j]){
                printf("Test Failed test 3 Materialiser from real table\n");
                return false;
            }
        }
    }
    
    
    //Test 5 Materialisation en passant par la map somme
    nombreAMaterialiser = 3;
    //Demande du nombre de requete à matérialiser en plus de la table de fait
    //Calcul des bénéfices
    requeteMaterialiser = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    stockerRequete(requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max);
    materialiserRequete(entierBinaireTestMaterialiserRequete4,taillesRequetes, requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max, 0, newTableFait);
    if(newTableFait.size() != tableFaitAttenduSommeMaterialiserDepuisMap.size()){
        printf("Test Failed test 1 Materialiser from map sum \n");
        return false;
    }
    for (int i = 0; i < newTableFait.size(); i++) {
        if(newTableFait[i].size() != tableFaitAttenduSommeMaterialiserDepuisMap[i].size()){
            printf("Test Failed test 2 Materialiser from map sum \n");
            return false;
        }
        for (int j = 0; j < newTableFait[i].size(); j++){
            if(newTableFait[i][j] != tableFaitAttenduSommeMaterialiserDepuisMap[i][j]){
                printf("Test Failed test 3 Materialiser from map sum \n");
                return false;
            }
        }
    }
    
    //Test 5 Materialisation en passant par la map max
    nombreAMaterialiser = 3;
    //Demande du nombre de requete à matérialiser en plus de la table de fait
    //Calcul des bénéfices
    requeteMaterialiser = calculBeneficeTotal(taillesRequetes, nombreAMaterialiser);
    stockerRequete(requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max);
    materialiserRequete(entierBinaireTestMaterialiserRequete4,taillesRequetes, requeteMaterialiser, tableFaitString, unordered_map_Sum, unordered_map_Max, 1, newTableFait);
    if(newTableFait.size() != tableFaitAttenduMaxMaterialiserDepuisMap.size()){
        printf("Test Failed test 1 Materialiser from map sum \n");
        return false;
    }
    for (int i = 0; i < newTableFait.size(); i++) {
        if(newTableFait[i].size() != tableFaitAttenduMaxMaterialiserDepuisMap[i].size()){
            printf("Test Failed test 2 Materialiser from map sum \n");
            return false;
        }
        for (int j = 0; j < newTableFait[i].size(); j++){
            if(newTableFait[i][j] != tableFaitAttenduMaxMaterialiserDepuisMap[i][j]){
                printf("Test Failed test 3 Materialiser from map sum \n");
                return false;
            }
        }
    }
    
    printf("Test réussi Materialiser Requete \n");
    return true;
}

//Test 6 Export Fichier
bool testExportFichier(string filePath){
    vector<vector<string>> tableFaitString = chargerFichiers(filePath);
    exportFichier(tableFaitString, "Plastico");
    vector<vector<string>> nouvelleTableFaitImporte = chargerFichiers("Plastico.csv");
    
    if(tableFaitString.size() != nouvelleTableFaitImporte.size()){
            printf("Test Failed test 1 Export Fichier \n");
            return false;
        }
        for (int i = 0; i < tableFaitString.size(); i++) {
            if(tableFaitString[i].size() != nouvelleTableFaitImporte[i].size()){
                printf("Test Failed test 2 Export Fichier \n");
                return false;
            }
            for (int j = 0; j < tableFaitString[i].size(); j++){
                if(tableFaitString[i][j] != nouvelleTableFaitImporte[i][j]){
                    printf("Test Failed test 3 Export Fichier \n");
                    return false;
                }
            }
        }
        
    printf("Test réussi Export Fichier \n");
    remove("Plastico.csv");
    return true;
}


