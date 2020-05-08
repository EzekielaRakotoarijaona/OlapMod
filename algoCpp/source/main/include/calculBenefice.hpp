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
#include <map>
#include "../include/nbEnregistrements.hpp"

using namespace std;

int calculBenefice(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

vector<int> requeteDep(int numeroRequete, vector<int>& taillesRequetes);

vector<int> calculBeneficeTotal(vector<int>& taillesRequetes, int nombreAMaterialiser);

int maxBenefice(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees);

int calculerBeneficeReel(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

int parQuiJeSuisCalculer(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

void afficherVector(vector<int>& vector);

void stockerRequete(vector<int>& requetesMaterialisees, vector<vector<string>>& table_FaitString, map<int,vector<vector<string>>>& map_Sum, map<int,vector<vector<string>>>& map_Max);

void afficherTableFait(vector<vector<string>>& tableFait);

void materialiser(vector<vector<string>> & tableFait, int requeteAMaterialiser, int typeOperation, vector<vector<string>> & newTableFait );

long findTuple(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire);

void insertInTableFait(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire);

void addition(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait);

void max(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait);

#endif /* calculBenefice_hpp */

