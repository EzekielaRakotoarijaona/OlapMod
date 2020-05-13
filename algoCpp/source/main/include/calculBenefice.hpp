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
#include <unordered_map> 

using namespace std;

int calculBenefice(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

vector<int> requeteDep(int numeroRequete, vector<int>& taillesRequetes);

vector<int> calculBeneficeTotal(vector<int>& taillesRequetes, int nombreAMaterialiser);

int maxBenefice(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees);

int calculerBeneficeReel(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

int parQuiJeSuisCalculer(vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, int numeroRequete);

void afficherVector(vector<int>& vector);

void stockerRequete(vector<int>& requetesMaterialisees, vector<vector<string>>& table_FaitString, unordered_map<int,vector<vector<string>>>& unordered_map_Sum, unordered_map<int,vector<vector<string>>>& unordered_map_Max);

void afficherTableFait(vector<vector<string>>& tableFait);

void materialiser(vector<vector<string>> & tableFait, int requeteAMaterialiser, int typeOperation, vector<vector<string>> & newTableFait );

long findTuple(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire);

void insertInTableFait(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire);

void addition(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait);

void max(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait);

void materialiserRequete(vector<int>& numeroDeRequete,vector<int>& taillesRequetes, vector<int>& requetesMaterialisees, vector<vector<string>>& table_FaitString, unordered_map<int,vector<vector<string>>>& unordered_map_Sum, unordered_map<int,vector<vector<string>>>& unordered_map_Max, int typeOperation,  vector<vector<string>>& newTableFait);

int conversionBinaireAEntier(vector<int>& numeroDeRequete);

#endif /* calculBenefice_hpp */

