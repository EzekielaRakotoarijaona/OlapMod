//
//  nbEnregistrements.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 01/04/2020.
//

#include "../include/nbEnregistrements.hpp"
#include <iostream>//c'est pour l'affichage
#include <vector>
#include<set>
#include <ctime>//ça sert pour la génération aléatoire
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <omp.h>
#include <map>
#include <iterator>

using namespace std; //ça permet d'utiliser la librairie stl

vector<vector<string>> chargerFichiers(string filePath) { // Extrait les valeurs sous format string du fichier CSV pour les mettre dans une matrice
    string ligne;
    ifstream file (filePath);
    vector<string> V2;
    vector<vector<string>> tableFait;
    char delim = ',';
    
    if (file.good())  // On vérifie si le fichier est ouvert
    {
        while (getline(file,ligne)) // On parcourt le fichier et on copie chaque valeur dans le vecteur V2
        {
            stringstream ss(ligne);
            string token;
            while (getline(ss, token, delim)) {
                V2.push_back(token);
            }
            tableFait.push_back(V2); // On remplit la matrice à partir du vecteur V2
            V2.clear();
        }
    }
    else{
        cout<<"erreur d'ouverture du fichier"<<endl;
        return tableFait;
    }
 
    for (int i = 0; i < tableFait.size(); i++) {
        for (int j = 0; j < tableFait[i].size(); j++)
            cout<< tableFait[i][j] << " ";
        cout<<" "<<endl;
    }
    return tableFait;
}

matrice conversion(vector<vector<string>> tableFaitString) {
    matrice tableFaitEntiers;
    map<string,int> ma_map;
    int map_valeur=0;

 tableFaitEntiers.resize(tableFaitString.size()-1);

 for (int i = 0; i < tableFaitString[0].size(); i++) {
        for (int j = 1; j < tableFaitString.size(); j++){
            if(ma_map.find(tableFaitString[j][i])==ma_map.end()){
                ma_map[tableFaitString[j][i]]=++map_valeur;
            }
            tableFaitEntiers[j-1].push_back(ma_map[tableFaitString[j][i]]);
        }
}
return tableFaitEntiers;
}

void generer(matrice& M, int l, int c) {
    //ça remplit une matrice M de l lignes et c colonnes
    M.resize(l);//on réserve l lignes
    for (auto i = 0; i < l; i++) M[i].resize(c);//chaque ligne est un vecteur de taille c
    //pour chaque case (i,j) de M, on lui affecte un nombre généré aléatoirement
    for (auto i = 0; i < M.size(); i++) {
        for (auto j = 0; j < M[i].size(); j++) {
            M[i][j] = rand()%23;// on génère un nombre compris entre 0 et 22
        }
    }
}

vector<int> extraire(vector<int>& V, vector<int>& W) {
    //ça extrait du vecteur V les valeurs qui se trouvent dans les cases
    //dont l'indice se trouve dans W. Les valeurs extraites sont mises dans Z qui est retourné
    //Ex: V=[1, 10, 3, 2, 5] et W=[0, 3]. Alors Z sera [1, 2] car ce sont les valeurs aux indices 0 et 3 dans V
    vector<int> Z;
    for (auto i = 0; i < W.size(); i++) Z.push_back(V[W[i]]);
    return Z;
}

int taille(matrice& M, vector<int>& W) {
    //ça compte le nombre de tuples de M, quand on considère les colonnes désignées dans W
    set<vector<int> > S;//ensemble où on met les vecteurs/tuples qu'on extraits de chaque ligne de M
    for (auto i = 0; i < M.size(); i++) {
        S.insert(extraire(M[i], W));
    }
    return S.size();
}

vector<int> convertirEnBinaire(int n, int c) {
    //prend un entier n et le convertit en un vecteur de bits 0/1 de taille c
    vector<int> V(c, 0); //V est de taille c et les cases sont intialisées à 0
    int indice = c - 1;//on remplit V en commençant par la fin
    while (n != 0) {
        V[indice] = n % 2;
        n = n / 2;
        indice = indice - 1;
    }
    return V;
}

vector<int> Binaire_colonnes(vector<int> &V) {
    //transforme un veteur de 0 et 1 en un vecteur d'indices de colonnes
    vector<int> W;
    for (auto i = 0; i < V.size(); i++) {
        if (V[i] == 1) {
            W.push_back(i);
        }
    }
    return W;
}

vector<int> toutes_les_tailles(matrice & M) {
    //affiche les taille de chaque combinaison de colonnes de M
    vector<int> resultat;
    int nbcolonnes = M[0].size();
    resultat.resize(pow(2, nbcolonnes));
    resultat[0] = 1;
    int combinaisonMax = pow(2, nbcolonnes) - 1;
    #pragma omp parallel for
    for (int combinaison = 1; combinaison <= combinaisonMax; combinaison++) {
        vector<int> V = convertirEnBinaire(combinaison, nbcolonnes);
        vector<int> W = Binaire_colonnes(V);
        resultat[combinaison] = taille(M, W);
    }
    for(int combinaison = 0; combinaison <= combinaisonMax; combinaison++ ) {
        cout << "la taille de la combinaison " << combinaison << " est " << resultat[combinaison] << endl;
    }
    return resultat; 
}
