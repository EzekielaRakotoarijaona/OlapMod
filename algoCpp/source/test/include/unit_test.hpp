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

static matrice tableFait = { {1,4,7,5},
                                               {1,5,8,8},
                                               {1,4,7,8},
                                               {1,5,8,3},
                                               {1,6,9,5},
                                               {2,6,9,2},
                                               {2,4,9,4},
                                               {2,4,8,6},
                                               {3,5,8,3},
                                               {3,6,7,1} };

static matrice_string tableFaitString = {{"USA","T1","TV","5"},
					{"USA","T2","PC","8"},
					{"USA","T1","TV","6"},
					{"USA","T2","PC","4"},
					{"USA","T3","DVD","8"},
					{"MEXICO","T3","DVD","6"},
					{"MEXICO","T1","DVD","2"},
					{"MEXICO","T1","PC","10"},
					{"CANADA","T2","PC","11"},
					{"CANADA","T3","TV","5"} };


static vector<int> stockMaterialisees = {7};


bool testCalculBenefice();
bool testRequeteDep();
bool testCalculBeneficeTotal();
bool testChargerFichier(string filePath);
bool testTaillesRequetes();
bool testIntegrationCalculBeneficeWorkflow(string filePath);
bool testMaterialiser(string filePath);

#endif
