#include <iostream>
#include <stdio.h>
#include "../include/unit_test.hpp"//c'est pour l'affichage
#include <vector>
#include <map>
#include <fstream>


using namespace std;

static const string FICHIER_CSV="../resources/table_fait_string.csv";
static const string FICHIER_CSV_TEST="../resources/table_fait_string_test.csv";

int main(int argc, char *argv[]) {
 if(argc > 1) {

 printf("Running Integration Test \n");
     if (testIntegrationCalculBeneficeWorkflow(argv[1])) {
         printf("Test réussi! \n");
     }
     else{
         printf("Integration Tests Failed\n");
         return EXIT_FAILURE;
     }
     return 0;
 }
    printf("Running Unit Tests \n");
    if (!testCalculBenefice()) {
        printf("Test failed Calcul Benefice\n");
        return EXIT_FAILURE;
    }
    if (!testRequeteDep()) {
        printf("Test failed  Requete Dep\n");
        return EXIT_FAILURE;
    }
   if (!testCalculBeneficeTotal()) {
       printf("Test failed  CalculBeneficeTotal\n");
       return EXIT_FAILURE;
    }
    if (!testTaillesRequetes()){
        printf("Test failed  Test tailles requetes\n");
        return EXIT_FAILURE;
    }
    if (!testChargerFichier(FICHIER_CSV_TEST)) {
        printf("Test failed  CalculBeneficeTotal\n");
        return EXIT_FAILURE;
    }
    if (!testMaterialiser(FICHIER_CSV_TEST)) {
        printf("Test failed  testMaterialiser \n");
        return EXIT_FAILURE;
    }
    if (!testStocker(FICHIER_CSV_TEST)) {
        printf("Test failed  testStocker \n");
        return EXIT_FAILURE;
    }
    if (!testConversionBinaireEntier()) {
        printf("Test failed  testConversionBinaire \n");
        return EXIT_FAILURE;
    }
    if (!testMateriliserRequete(FICHIER_CSV_TEST)) {
        printf("Test failed  testMaterialiserRequete \n");
        return EXIT_FAILURE;
    }
    printf("Test réussi! \n");
    printf("Test unitaires réussis! \n");
    return 0;

}
