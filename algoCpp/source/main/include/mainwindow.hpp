//
//  mainwindow.hpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 20/04/2020.
//

#ifndef mainwindow_hpp
#define mainwindow_hpp

#include <stdio.h>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <vector>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include "../include/nbEnregistrements.hpp"
#include "../include/calculBenefice.hpp"
#include <unordered_map>
#include <QProgressDialog>
#include <QMessageBox>


namespace Ui {
   class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
       explicit MainWindow(QWidget *parent = 0);
    private:
        void initTableFaitView();
        void initiChargerLayout();
        void initRequeteLayout();
        void initExporterLayout();
        void nbMterialisationLayout();
        void initTableTailleRequetesWidget();
        vector<string> split(const string &s, char delim);
        void runCaculRequete();
        void runChargementFichier();
    signals:
        void endCalculRequete(int value);
        void endChargementFichier(int value);
    private slots:
        void handleButton();
        void calculRequetesAMateriliser();
        void request();
        void onClickChampsComboBox(int);
        void tailleMaxVector(int);
        void effacerListeChamps();
        void displayPopupEndCalculRequete(int value);
        void displayPopupEndChargementFichier(int value);
    private:
        QLabel *title;
        QLabel *titleTableFait;
        QLabel *titleTableGeneree;
        QLabel *chargement;
        QProgressDialog* bar;
        QMessageBox *msgBox;
        QGridLayout* mainLayout;
        QScrollArea* mainScrollArea;
        QGridLayout* chargerLayout;
        QLabel* chargerBack;
        QGridLayout* nbMaterialiserLayout;
        QLabel* textMemoire;
        QLabel* nbMaterialiserBack;
        QGridLayout* requeteLayout;
        QLabel* exporterBack;
        QGridLayout* exporterLayout;
        QPushButton * exporterButton;
        QLabel* requeteBack;
        QTextEdit* listeChampsRetenu;
        QPushButton *validerRequete;
        QPushButton *effacerChamps;
        QLabel *nbRequetesAMaterialiserLabel;
        QComboBox *nbRequetesAMaterialiserBox;
        QComboBox *champsRequetesComboBox;
        QComboBox *fonctionAggregationComboBox;
        QPushButton *nbRequetesAMaterialiserButton;
        QPushButton *m_button;
        QTableWidget *tableFaitWidget;
        QTableWidget *tableTailleRequetesWidget;
        QTableWidget *tableFaitRequeteWidget;
        QString dirPath;
        QString fileName;
        QFont *baloo;

    
        vector<vector<string>> tableFaitString;
        vector<int> taillesRequetes;
        vector<int> requetesMaterialise;
        matrice tableFait;
        unordered_map<int,vector<vector<string>>> map_Sum;
        unordered_map<int,vector<vector<string>>> map_Max;
        vector<vector<string>> tableFaitRequete;
        int espaceMemoire = 0;
        int nbAMateriliser = 0;
        int espaceMemoireReel = 0;
        
};

#endif /* mainwindow_hpp */
