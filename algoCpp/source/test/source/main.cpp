
#include <iostream>
#include "../include/unit_test.hpp"//c'est pour l'affichage
#include <vector>
#include <map>



int main() {
    
    map<int,vector<int>> first;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if((j & i) == j || j == 8) {
                first[i].push_back(j);
            }
            
        }
    }
    
    for (int i = 1; i < 9; i++) {
        printf("%d peut calculer les noeuds : ", i);
        vector<int> value = first[i];
        for(int j = 1; j < value.size(); j++){
            printf("%d ; ", value[j]);
        }
        printf("\n");
        
    }
    
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
