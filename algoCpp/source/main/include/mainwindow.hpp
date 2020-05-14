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
    private slots:
       void handleButton();
       void calculRequetesAMateriliser();
       void request();
    private:
        QLabel *title;
        QLabel *titleTableFait;
        QLabel *titleTableGeneree;
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
    
        vector<vector<string>> tableFaitString;
        vector<int> taillesRequetes;
        vector<int> requetesMaterialise;
        matrice tableFait;
        unordered_map<int,vector<vector<string>>> map_Sum;
        unordered_map<int,vector<vector<string>>> map_Max;
        vector<vector<string>> tableFaitRequete;
        
};

#endif /* mainwindow_hpp */
    
