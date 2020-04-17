#ifndef unit_test_hpp
#define unit_test_hpp
#include "../../main/include/nbEnregistrements.hpp"

#include <stdio.h>
#include <iostream>//c'est pour l'affichage
#include <vector>


using namespace std;
static const vector<int> resultatAttenduTestTaillesRequetes={1, 3, 3, 6, 3, 7, 7, 8};


static vector<int> taillesRequetes = {1, 3, 5, 6, 7, 6, 7, 8};
static vector<int> requetesMaterialisees = {7};
static vector<int> taillesRequetes2 = {1, 20, 15, 50, 30, 40, 75, 100};

static matrice tableFait = { {1,4,7},
                                               {1,5,8},
                                               {1,4,7},
                                               {1,5,8},
                                               {1,6,9},
                                               {2,6,9},
                                               {2,4,9},
                                               {2,4,8},
                                               {3,5,8},
                                               {3,6,7} };

bool testCalculBenefice();
bool testRequeteDep();
bool testCalculBeneficeTotal();
bool testChargerFichier(string filePath);
bool testTaillesRequetes();

#endif
