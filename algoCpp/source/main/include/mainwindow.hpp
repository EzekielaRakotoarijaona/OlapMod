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
#include <QFont>
#include <QLineEdit>
#include <QMenuBar>


namespace Ui {
   class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
       explicit MainWindow(QWidget *parent = 0);
    private:
        //Chargement de la Vue de Table de Faits
        void initTableFaitView();
        //Chargement de la Vue d'import de fichier
        void initiChargerLayout();
        //Chargement de la Vue de requetage
        void initRequeteLayout();
        //Chargement de la Vue d'export de fichier
        void initExporterLayout();
        //Chargement de la Vue de matérialisation
        void nbMterialisationLayout();
        //Chargement de la Vue de table de requetée
        void initTableTailleRequetesWidget();
        vector<string> split(string &s, char delim);
        //Calcul des matérialisation dans un thread concurrent
        void runCaculRequete();
        //Chargement de fichier dans un concurrent thread
        void runChargementFichier();
        //Mise à jour de la barre de progression Matérialisation via un thread concurrent
        void processMaterialisation();
        //Mise à jour de la barre de progression Chargement fichier via un thread concurrent
        void processChargementFichier();
        //Export de fichier dans un concurrent thread
        void exporter();
    signals:
        //Les différents signaux émis pour mettre à jour les barres de progression
        void endCalculRequete(long value);
        void endChargementFichier(long value);
        void endExportFichier(long value);
        void endRequest(long value);
    private slots:
        //Les différentes fonctions liées au boutons et aux événements
        void handleButton();
        void calculRequetesAMateriliser();
        void request();
        void onClickChampsComboBox(int);
        void tailleMaxVector(QString);
        void effacerListeChamps();
        void displayPopupEndCalculRequete(long value);
        void displayPopupEndChargementFichier(long value);
        void exporterButtonLaunch();
        void displayPopupExporter(long value);
        void doRequest();
        void requestBarUpdate(long value);
        void info();
        void help();
    private:
        //Variable de mise à l'échelle pour les résolutions d'écran différentes
        const int baseWidth = 1440;
        const int baseHeigth = 900;
        double scaleWidthRatio = 1;
        double scaleHeigthRatio = 1;
    
        //Widget modifiable de la fenêtre
        QMenuBar *menuBar;
        QLabel *title;
        QLabel *titleTableFait;
        QLabel *titleTableGeneree;
        QLabel *chargement;
        QLabel* tempsReq;
        QLabel* memoireReelle;
        QLabel* reqMat;
        QLabel* memoireReelleVal;
        QLabel* reqMatVal;
        QLabel* tailleTableFait;
        QLabel* tailleResultatRequete;
        QProgressDialog* bar;
        QProgressDialog* barRequest;
        QProgressDialog* barExport;
        QProgressDialog* barMat;
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
        QLineEdit *nbRequetesAMaterialiserBox;
        QComboBox *champsRequetesComboBox;
        QComboBox *fonctionAggregationComboBox;
        QPushButton *nbRequetesAMaterialiserButton;
        QPushButton *m_button;
        QTableWidget *tableFaitWidget;
        QTableWidget *tableTailleRequetesWidget;
        QTableWidget *tableFaitRequeteWidget;
        QString dirPath;
        QString fileName;
        QFont* baloo;         // variable pour la police d'écriture
        QStatusBar* statusBar;
        QString saveDirPath;
        QString saveFileName;
    
        //Les Objets nécéssaire à la partie traitement de données 
        vector<vector<string>> tableFaitString;
        vector<long> taillesRequetes;
        vector<long> requetesMaterialise;
        matrice tableFait;
        unordered_map<long,vector<vector<string>>> map_Sum;
        unordered_map<long,vector<vector<string>>> map_Max;
        vector<vector<string>> tableFaitRequete;
        long espaceMemoire = 0;
        long nbAMateriliser = 0;
        long espaceMemoireReel = 0;
        double tempsChargement = 0.0;
        double tempsMaterialisation = 0.0;
        double tempsRequete = 0.0;
        bool endCalculReq = false;
        bool endChargeFichier = false;
        
};

#endif /* mainwindow_hpp */
