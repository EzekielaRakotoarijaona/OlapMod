
#include <iostream>
#include "../include/unit_test.hpp"//c'est pour l'affichage
#include <vector>
#include <map>



int main() {
    
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
    
}
