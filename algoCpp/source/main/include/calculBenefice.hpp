//
//  calculBenefice.hpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 07/04/2020.
//

#ifndef calculBenefice_hpp
#define calculBenefice_hpp

#include <stdio.h>
#include <vector>

using namespace std;

int calculBenefice(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

vector<int> requeteDep(int numeroRequete, vector<int>& taillesRequetes);

vector<int> calculBeneficeTotal(vector<int>& taillesRequetes, int nombreAMaterialiser);

int maxBenefice(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees);

int calculerBeneficeReel(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

int parQuiJeSuisCalculer(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

void afficherVector(vector<int>& vector);


#endif /* calculBenefice_hpp */

