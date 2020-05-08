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


static vector<int> stockMaterialisees = {7};

static vector<vector<string>> tableFaitAttenduSomme = { {"QUATER", "PRODUCT", "#UNITS"}, {"T1","TV","11"},
{"T2","PC","23"},
{"T3","DVD","14"},
{"T1","DVD","2"},
{"T1","PC","10"},
{"T3","TV","5"}};

static vector<vector<string>> tableFaitAttenduMax = {{"QUATER", "PRODUCT", "#UNITS"}, {"T1","TV","6"},
{"T2","PC","11"},
{"T3","DVD","8"},
{"T1","DVD","2"},
{"T1","PC","10"},
{"T3","TV","5"}};


bool testCalculBenefice();
bool testRequeteDep();
bool testCalculBeneficeTotal();
bool testChargerFichier(string filePath);
bool testTaillesRequetes();
bool testIntegrationCalculBeneficeWorkflow(string filePath);
bool testMaterialiser(string filePath);
bool testStocker(string filePath);

#endif
