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
    resultatAttendu = 4;
    resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 2);
    
    if (resultatReel != resultatAttendu) {
        printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
        
        //Test 3 : On rajoute un noeud matérialisé de plus : noeud 3
        requetesMaterialisees.push_back(3);
        resultatAttendu = 1;
        resultatReel = calculBenefice(taillesRequetes, requetesMaterialisees, 2);
        if(resultatAttendu == resultatReel)
        {
            printf("Test successful : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
            return true;
        }
    }
    
    printf("Test failed : Resultat attendu : %d / resultat réel : %d \n", resultatAttendu, resultatReel);
    return false;
    
    
}
