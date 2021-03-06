//
//  nbEnregistrements.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 01/04/2020.
//

#include "../include/nbEnregistrements.hpp"
#include <iostream>//c'est pour l'affichage
#include <vector>
#include <set>
#include <ctime>//ça sert pour la génération aléatoire
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <omp.h>
#include <map>
#include <iterator>
#include <unordered_set>
#include <codecvt>


using namespace std; //ça permet d'utiliser la librairie stl


//On définit une fonction de hash qui associe un entier à un vecteur d'entier
struct VectorHash {
    size_t operator()(const std::vector<long>& v) const {
        std::hash<long> hasher;
        size_t seed = 0;
        for (long i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};


//On définit le type MySet qui est un unoredred_set de vecteurs d'entiers. Ces vecteurs sont hachés en utilisant
//la fonction VectorHash définie ci-dessus

using MySet = std::unordered_set<std::vector<long>, VectorHash>;


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
 
    for (long i = 0; i < tableFait.size(); i++) {  // On parcourt la matrice et affiche ses valeurs
        for (long j = 0; j < tableFait[i].size(); j++)
            cout<< tableFait[i][j] << " ";
        cout<<" "<<endl;
    }
    long size = tableFait[0].size();
    for(long i = 1; i<tableFait.size(); i++) {
        if(tableFait[i].size() != size || valid_number(tableFait[i][tableFait[i].size() - 1]) == 0) {
            tableFait.clear();
            return tableFait;
        }
    }
    return tableFait;
}

int valid_number(string str)
{
    int i = 0, j = str.length() - 1;
  
    // Handling whitespaces
    while (i < str.length() && str[i] == ' ')
        i++;
    while (j >= 0 && str[j] == ' ')
        j--;
  
    if (i > j)
        return 0;
  
    // if string is of length 1 and the only
    // character is not a digit
    if (i == j && !(str[i] >= '0' && str[i] <= '9'))
        return 0;
  
    // If the 1st char is not '+', '-', '.' or digit
    if (str[i] != '.' && str[i] != '+'
        && str[i] != '-' && !(str[i] >= '0' && str[i] <= '9'))
        return 0;
  
    // To check if a '.' or 'e' is found in given
    // string. We use this flag to make sure that
    // either of them appear only once.
    bool flagDotOrE = false;
  
    for (i; i <= j; i++) {
        // If any of the char does not belong to
        // {digit, +, -, ., e}
        if (str[i] != 'e' && str[i] != '.'
            && str[i] != '+' && str[i] != '-'
            && !(str[i] >= '0' && str[i] <= '9'))
            return 0;
  
        if (str[i] == '.') {
            // checks if the char 'e' has already
            // occurred before '.' If yes, return 0.
            if (flagDotOrE == true)
                return 0;
  
            // If '.' is the last character.
            if (i + 1 > str.length())
                return 0;
  
            // if '.' is not followed by a digit.
            if (!(str[i + 1] >= '0' && str[i + 1] <= '9'))
                return 0;
        }
  
        else if (str[i] == 'e') {
            // set flagDotOrE = 1 when e is encountered.
            flagDotOrE = true;
  
            // if there is no digit before 'e'.
            if (!(str[i - 1] >= '0' && str[i - 1] <= '9'))
                return 0;
  
            // If 'e' is the last Character
            if (i + 1 > str.length())
                return 0;
  
            // if e is not followed either by
            // '+', '-' or a digit
            if (str[i + 1] != '+' && str[i + 1] != '-'
                && (str[i + 1] >= '0' && str[i] <= '9'))
                return 0;
        }
    }
    /* If the string skips all above cases, then
    it is numeric*/
    return 1;
}

matrice conversion(vector<vector<string>> tableFaitString) {  // A partir de la tableFaitString qui a été généré dans la fonction d'avant
                                                               // on convertit les données "string" en "entier" à partir d'une map
                                                               // et on les insère dans une nouvelle matrice tableFaitEntier
    matrice tableFaitEntiers; 
    map<string,long> ma_map;
    long map_valeur=0;

    tableFaitEntiers.resize(tableFaitString.size()-1);  // On initialise la taille de tableFaitEntier
    if(tableFaitString.size() < 2) {
        return tableFaitEntiers;
    }

 for (long i = 0; i < tableFaitString[0].size()-1; i++) {  // On parcourt la tableFaitString sans prendre en compte la dernière colonne (#units) qui n'a pas besoin d'être convertie
        for (long j = 1; j < tableFaitString.size(); j++){
            if(ma_map.find(tableFaitString[j][i])==ma_map.end()){ // L'objectif est d'associer une valeur entière à chaque string différent
                                                                  // On utilise une MAP et on ajoute une ligne d'association clé-valeur à la map
                                                                  // Si la valeur de la tableFaitString n'est pas déja dans la map  
                                                                   // On ajoute une ligne d'association clé-valeur à la map 
                                                                   // Exemple : La clé correspond au string : "USA" et la valeur aux entier : "1"
                ma_map[tableFaitString[j][i]]=++map_valeur;  
            }
            tableFaitEntiers[j-1].push_back(ma_map[tableFaitString[j][i]]);
        }
}
return tableFaitEntiers; // On retourne la table de fait avec des valeurs entières
}

void generer(matrice& M, long l, long c) {
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

vector<long> extraire(vector<long>& V, vector<long>& W) {
    //ça extrait du vecteur V les valeurs qui se trouvent dans les cases
    //dont l'indice se trouve dans W. Les valeurs extraites sont mises dans Z qui est retourné
    //Ex: V=[1, 10, 3, 2, 5] et W=[0, 3]. Alors Z sera [1, 2] car ce sont les valeurs aux indices 0 et 3 dans V
    vector<long> Z;
    for (auto i = 0; i < W.size(); i++) Z.push_back(V[W[i]]);
    return Z;
}

long taille(matrice& M, vector<long>& W) {
//ça compte le nombre de tuples de M, quand on considère les colonnes désignées dans W
    //set<vector<long>> S;
    //ensemble où on met les vecteurs/tuples qu'on extraits de chaque ligne de M
    MySet S;
    for (long i = 0; i < M.size(); i++) {
        S.insert(extraire(M[i], W));
    }
    return S.size();
}

vector<long> convertirEnBinaire(long n, long c) {
    //prend un entier n et le convertit en un vecteur de bits 0/1 de taille c
    vector<long> V(c, 0); //V est de taille c et les cases sont intialisées à 0
    long indice = c - 1;//on remplit V en commençant par la fin
    while (n != 0) {
        V[indice] = n % 2;
        n = n / 2;
        indice = indice - 1;
    }
    return V;
}

vector<long> Binaire_colonnes(vector<long> &V) {
    //transforme un veteur de 0 et 1 en un vecteur d'indices de colonnes
    vector<long> W;
    for (auto i = 0; i < V.size(); i++) {
        if (V[i] == 1) {
            W.push_back(i);
        }
    }
    return W;
}

vector<long> toutes_les_tailles(matrice & M) {
    //affiche les taille de chaque combinaison de colonnes de M
    vector<long> resultat;
    long nbcolonnes = M[0].size();
    resultat.resize(pow(2, nbcolonnes));
    resultat[0] = 1;
    long combinaisonMax = pow(2, nbcolonnes) - 1;
    #pragma omp parallel for
    for (long combinaison = 1; combinaison <= combinaisonMax; combinaison++) {
        vector<long> V = convertirEnBinaire(combinaison, nbcolonnes);
        vector<long> W = Binaire_colonnes(V);
        resultat[combinaison] = taille(M, W);
    }
    for(long combinaison = 0; combinaison <= combinaisonMax; combinaison++ ) {
        cout << "la taille de la combinaison " << combinaison << " est " << resultat[combinaison] << endl;
    }
    return resultat; 
}



