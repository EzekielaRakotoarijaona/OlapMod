#include "../include/unit_test.hpp"
#include "../../main/include/calculBenefice.hpp"

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
                        resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 8);
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
    vector<int> resultatAttendu = {1,2,3,4,5,6,7,8};
    vector<int> resultatReel = requeteDep(7, taillesRequetes);
    sort(resultatReel.begin(), resultatReel.end());
    if(resultatAttendu != resultatReel){
        printf("Test Failed\n");
        return false;
    }
    resultatAttendu = {2,4,6,8};
    resultatReel = requeteDep(6, taillesRequetes);
    sort(resultatReel.begin(), resultatReel.end());
    if(resultatAttendu != resultatReel){
        printf("Test Failed \n");
        return false;
    }
    resultatAttendu = {8};
    resultatReel = requeteDep(8, taillesRequetes);
    sort(resultatReel.begin(), resultatReel.end());
    if(resultatAttendu != resultatReel){
        printf("Test Failed \n");
        return false;
    }
    return true;
}
