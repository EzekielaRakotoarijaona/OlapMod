
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
        printf("Test failed \n");
    }
    printf("Test réussi! \n");
    return 0;
    
}
