#ifndef unit_test_hpp
#define unit_test_hpp

#include <stdio.h>
#include <iostream>//c'est pour l'affichage
#include <vector>

using namespace std;

static const vector<int> taillesRequetes = {0, 3, 5, 6, 7, 6, 7, 8, 1};
static vector<int> requetesMaterialisees = {7};
static const vector<int> taillesRequetes2 = {0, 20, 15, 50, 30, 40, 75, 100, 1};

static const vector<vector<int>> tableFait = { {1,4,7},
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

#endif
