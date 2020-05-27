//
//  nbEnregistrements.hpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 01/04/2020.
//

#ifndef nbEnregistrements_hpp
#define nbEnregistrements_hpp

#include <stdio.h>
#include <iostream>//c'est pour l'affichage
#include <vector>
#include<set>
#include <ctime>//ça sert pour la génération aléatoire
#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_map>


using namespace std;//ça permet d'utiliser la librairie stl

typedef vector<vector<long>> matrice;//on définit le type matrice
typedef vector<vector<string>> matrice_string;//on définit le type matrice pour des string

vector<vector<string>> chargerFichiers(string filePath);

int valid_number(string str);

matrice conversion(vector<vector<string>> tableFaitString);

void generer(matrice& M, long l, long c);

vector<long> extraire(vector<long>& V, vector<long>& W);

long taille(matrice& M, vector<long>& W);

vector<long> convertirEnBinaire(long n, long c);

vector<long> Binaire_colonnes(vector<long> &V);

vector<long> toutes_les_tailles(matrice & M);


#endif /* nbEnregistrements_hpp */
