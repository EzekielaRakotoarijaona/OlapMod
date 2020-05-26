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

long calculBenefice(vector<long>& taillesRequetes, vector<long>& requetesMaterialisees, long numeroRequete);

vector<long> requeteDep(long numeroRequete, vector<long>& taillesRequetes);

vector<long> calculBeneficeTotal(vector<long>& taillesRequetes, long nombreAMaterialiser);

long maxBenefice(vector<long>& taillesRequetes, vector<long>& requetesMaterialisees);

long calculerBeneficeReel(vector<long>& taillesRequetes, vector<long>& requetesMaterialisees, long numeroRequete);

long parQuiJeSuisCalculer(vector<long>& taillesRequetes, vector<long>& requetesMaterialisees, long numeroRequete);

void afficherVector(vector<long>& vector);

void stockerRequete(vector<long>& requetesMaterialisees, vector<vector<string>>& table_FaitString, unordered_map<long,vector<vector<string>>>& unordered_map_Sum, unordered_map<long,vector<vector<string>>>& unordered_map_Max);

void afficherTableFait(vector<vector<string>>& tableFait);

void materialiser(vector<vector<string>> & tableFait, long requeteAMaterialiser, long typeOperation, vector<vector<string>> & newTableFait );

long findTuple(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire, vector<long> &entier_Binaire, long size);

void insertInTableFait(vector<vector<string>>& newTableFait, vector<string>& tableFaitIntermediaire, vector<long> &entier_Binaire);

void addition(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait);

void max(vector<vector<string>>& tableFait, vector<vector<string>>& newTableFait, long positionTableFait, long positionNewTableFait);

void materialiserRequete(vector<string>& numeroDeRequete,vector<long>& taillesRequetes, vector<long>& requetesMaterialisees, vector<vector<string>>& table_FaitString, unordered_map<long,vector<vector<string>>>& unordered_map_Sum, unordered_map<long,vector<vector<string>>>& unordered_map_Max, long typeOperation,  vector<vector<string>>& newTableFait);

long conversionBinaireAEntier(vector<long>& numeroDeRequete);

bool vectorContains(long value, vector<long> vector);

vector<long> fromStringToBinary(vector<string> stringRequest, vector<vector<string>> &tableFaitString);

long espaceMemoireUtilise(vector<long> &taillesRequetes, vector<long>& requetesMaterialisees);

long espaceMemoirePrevu(vector<long> &taillesRequetes, long nbPrevu);

void exportFichier(vector<vector<string>> tableFait, string nomFichier);

#endif /* calculBenefice_hpp */

