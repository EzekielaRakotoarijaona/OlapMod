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


using namespace std;//ça permet d'utiliser la librairie stl

vector<vector<string>> chargerFichier();

typedef vector<vector<int>> matrice;//on définit le type matrice

void generer(matrice& M, int l, int c);

vector<int> extraire(vector<int>& V, vector<int>& W);

int taille(matrice& M, vector<int>& W);

vector<int> convertirEnBinaire(int n, int c);

vector<int> Binaire_colonnes(vector<int> &V);

void toutes_les_tailles(matrice & M);


#endif /* nbEnregistrements_hpp */
