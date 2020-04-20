#include "../include/unit_test.hpp"
#include "../../main/include/calculBenefice.hpp"
#include "../../main/include/nbEnregistrements.hpp"
#include <iostream>
#include <stdio.h>

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
        printf("Test Failed test Charger fichier \n");
        return false;
    }
    for (int i = 0; i < tableFait.size(); i++) {
        if(tableFait[i].size() != resultatReel[i].size()){
            printf("Test Failed test Charger fichier \n");
            return false;
        }
        for (int j = 0; j < tableFait[i].size(); j++){
            if(tableFait[i][j] != resultatReel[i][j]){
                printf("Test Failed test Charger fichier \n");
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



