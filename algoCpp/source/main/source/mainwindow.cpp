//
//  mainwindow.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 20/04/2020.
//

#include "../include/mainwindow.hpp"
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include "../include/nbEnregistrements.hpp"
#include "../include/calculBenefice.hpp"
#include <iostream>
#include <fstream>
#include <QHeaderView>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMovie>
#include <sstream>
#include <QProgressDialog>
#include <thread>
#include <QtConcurrent/QtConcurrent>
#include <QFont>
#include <QFontDatabase>



using namespace std;

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
    
    int id = QFontDatabase::addApplicationFont("../ui_resources/Baloo-Regular-webfont.ttf");// AJOUT CE
    if(id!=-1) {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);// AJOUT CE
        baloo = new QFont(family);
    }
    else baloo = new QFont(QString::fromStdString("Baloo"));
    
    initiChargerLayout();
    initRequeteLayout();
    nbMterialisationLayout();
    initExporterLayout();

    
   tempsReq = new QLabel(this);
   tempsReq->setGeometry(QRect(QPoint(100, 30),
    QSize(200, 50)));
   tempsReq->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   tempsReq->setFont(*baloo);// AJOUT CE
   tempsReq->setStyleSheet("font-weight:medium; font-size:14pt");
   tempsReq->setText(QString::fromStdString(to_string(tempsRequete)) + "s");
    tempsReq->setAlignment(Qt::AlignTop | Qt::AlignRight);
    tempsReq->setFrameShape(QFrame::HLine);
    tempsReq->setFrameStyle(QFrame::NoFrame);
    
    
   title = new QLabel(this);
   title->setGeometry(QRect(QPoint(100, 30),
    QSize(200, 50)));
   title->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   title->setFont(*baloo);// AJOUT CE
   title->setStyleSheet("font-weight:medium; font-size:28pt"); // AJOUT CE
   title->setText("QueryOptimizer");// AJOUT CE
    title->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    title->setFrameShape(QFrame::HLine);
    title->setFrameStyle(QFrame::NoFrame);
    
    titleTableFait = new QLabel(this);
    titleTableFait->setGeometry(QRect(QPoint(100, 30),
     QSize(200, 50)));
    titleTableFait->setFrameStyle(QFrame::Panel | QFrame::Sunken);

   titleTableFait->setFont(*baloo);// AJOUT CE
   titleTableFait->setStyleSheet("font-weight:medium; font-size:16pt;color:grey"); // AJOUT CE
   titleTableFait->setText("Table de fait - Originale");// AJOUT CE
     titleTableFait->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     titleTableFait->setFrameShape(QFrame::HLine);
     titleTableFait->setFrameStyle(QFrame::NoFrame);
    
    titleTableGeneree = new QLabel(this);
    titleTableGeneree->setGeometry(QRect(QPoint(100, 30),
     QSize(200, 50)));
    titleTableGeneree->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   titleTableGeneree->setFont(*baloo);// AJOUT CE
   titleTableGeneree->setStyleSheet("font-weight:medium; font-size:16pt;color:grey"); // AJOUT CE
   titleTableGeneree->setText("Table de fait - Générée");// AJOUT CE
     titleTableGeneree->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     titleTableGeneree->setFrameShape(QFrame::HLine);
     titleTableGeneree->setFrameStyle(QFrame::NoFrame);
    
    
    mainLayout = new QGridLayout();
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->setRowMinimumHeight(4, 410);
    mainLayout->setRowMinimumHeight(0, 50);
    mainLayout->setVerticalSpacing(20);
    mainLayout->setHorizontalSpacing(30);
    mainLayout->addWidget(title,0,0,Qt::AlignCenter| Qt::AlignLeft );
    mainLayout->addWidget(chargerBack,1,0);
    mainLayout->addLayout(chargerLayout, 1,0);
    mainLayout->addWidget(requeteBack,1,1);
    mainLayout->addLayout(requeteLayout, 1,1);
    mainLayout->addWidget(nbMaterialiserBack, 1,2);
    mainLayout->addLayout(nbMaterialiserLayout, 1,2);
    mainLayout->addWidget(exporterBack, 1,3);
    mainLayout->addLayout(exporterLayout, 1,3);
    mainLayout->addWidget(titleTableFait, 2,0);
    mainLayout->addWidget(titleTableGeneree, 2,2);
    mainLayout->addWidget(tempsReq, 2, 3);

    QWidget* contentWidget = new QWidget();
    contentWidget->setLayout(mainLayout);

    mainScrollArea = new QScrollArea();
    mainScrollArea->setWidget(contentWidget);
    mainScrollArea->resize(2000,4000);

    setCentralWidget(mainScrollArea);
    msgBox = new QMessageBox();
    tableFaitWidget = new QTableWidget();
    tableFaitWidget->setMaximumWidth(630);
    tableFaitWidget->setMinimumWidth(630);
    tableFaitWidget->setMaximumHeight(410);
    tableFaitWidget->setMinimumHeight(410);
    mainLayout->addWidget(tableFaitWidget, 4,0,1,2, Qt::AlignTop| Qt::AlignLeft);
    
    
    tableFaitRequeteWidget = new QTableWidget();
    tableFaitRequeteWidget->setMaximumWidth(630);
    tableFaitRequeteWidget->setMinimumWidth(630);
    tableFaitRequeteWidget->setMaximumHeight(410);
    tableFaitRequeteWidget->setMinimumHeight(410);
    mainLayout->addWidget(tableFaitRequeteWidget, 4,2,1,2, Qt::AlignTop| Qt::AlignLeft);
}

void MainWindow::handleButton()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Charger une table de fait ..."),"../resources",tr("BDD (*.csv)"));
    if(file.isEmpty()) {
        return;
    }
    bar = new QProgressDialog();
    bar->setRange(0,100);
    bar->setAutoClose(true);
    bar->setCancelButtonText(QString());
    bar->show();
    mainLayout->addWidget(bar,0,3, Qt::AlignTop | Qt::AlignRight);
    QFileInfo fileInfo(file);
    dirPath = fileInfo.filePath(); // Path vers le fichier
    fileName = fileInfo.fileName();
    string filePath = dirPath.toUtf8().constData();
    connect(this, SIGNAL(endChargementFichier(int)), this, SLOT(displayPopupEndChargementFichier(int)), Qt::BlockingQueuedConnection);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::runChargementFichier );
}

void MainWindow::initTableFaitView(){
    if(this->tableFaitString.size() > 1000) {
        tableFaitWidget->setRowCount(1000);
    }
    else tableFaitWidget->setRowCount(this->tableFaitString.size()-1);
    tableFaitWidget->setColumnCount(this->tableFaitString[0].size());
    for(int i = 0; i<tableFaitString[0].size(); i++) {
       tableFaitWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromStdString(tableFaitString[0][i])));
    }
    for(int i = 1; i<tableFaitString.size(); i++) {
        for(int j = 0; j<tableFaitString[0].size(); j++){
            QTableWidgetItem *item = tableFaitWidget->item(-1, j);
            if(!item) {
                item = new QTableWidgetItem();
                tableFaitWidget->setItem(i-1, j, item);
            }
            item->setText(QString::fromStdString(tableFaitString[i][j]));
        }
    }
    tableFaitWidget->setGeometry(QRect(QPoint(40, 200),
    QSize(1500, 300)));
    tableFaitWidget->setMaximumWidth(630);
    tableFaitWidget->setMinimumWidth(630);
    tableFaitWidget->setMaximumHeight(410);
    tableFaitWidget->setMinimumHeight(410);
    tableFaitWidget->resizeColumnsToContents();
    tableFaitWidget->resizeRowsToContents();
    
    mainLayout->addWidget(tableFaitWidget, 4,0,1,2, Qt::AlignTop| Qt::AlignLeft);
    delete champsRequetesComboBox;
    champsRequetesComboBox =  new QComboBox(this);
    champsRequetesComboBox->addItem("");
    for(int i = 0; i< tableFaitString[0].size() - 1; i++) {
        champsRequetesComboBox->addItem(QString::fromStdString(tableFaitString[0][i]));
    }
    champsRequetesComboBox->setGeometry(QRect(QPoint(650, 100),
    QSize(100, 50)));
    connect(champsRequetesComboBox,SIGNAL (currentIndexChanged(int)),this,SLOT(onClickChampsComboBox(int)));
    requeteLayout->addWidget(champsRequetesComboBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
}

void MainWindow::initTableTailleRequetesWidget(){
    requetesMaterialise.resize(1);
    requetesMaterialise[0] = taillesRequetes.size()-1;
    delete nbRequetesAMaterialiserBox;
    nbRequetesAMaterialiserBox =  new QComboBox(this);
    for(int i = 0; i< taillesRequetes.size(); i++) {
        nbRequetesAMaterialiserBox->addItem(QString::fromStdString(to_string(i)));
    }
    connect(nbRequetesAMaterialiserBox,SIGNAL (currentIndexChanged(int)),this,SLOT(tailleMaxVector(int)));
    nbRequetesAMaterialiserBox->setGeometry(QRect(QPoint(650, 100),
    QSize(100, 50)));
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
}


void MainWindow::calculRequetesAMateriliser() {
    if(tableFaitString.size() == 0) {
        msgBox->setText("Commencez par importer une table avant d'optimiser");
        msgBox->show();
        return;
    }
    bar = new QProgressDialog();
    bar->setAutoClose(true);
    bar->setCancelButtonText(QString());
    bar->setRange(0,100);
    bar->setValue(0);
    mainLayout->addWidget(bar,0,3, Qt::AlignTop| Qt::AlignRight);
    connect(this, SIGNAL(endCalculRequete(int)), this, SLOT(displayPopupEndCalculRequete(int)), Qt::BlockingQueuedConnection);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::runCaculRequete );
}

void MainWindow::initiChargerLayout() {
    chargerLayout = new QGridLayout();
    
    QLabel* chiffre1 = new QLabel(this);
    chiffre1->setGeometry(QRect(QPoint(0,0),
     QSize(50, 50)));
    chiffre1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QPixmap pixmapChiffre1("../ui_resources/chiffre_1.png");
    chiffre1->setPixmap(pixmapChiffre1.scaled(chiffre1->width(),chiffre1->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    chiffre1->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    chiffre1->setFrameShape(QFrame::HLine);
    chiffre1->setFrameStyle(QFrame::NoFrame);

    QLabel* selectionFichier = new QLabel(this);
    selectionFichier->setGeometry(QRect(QPoint(0,0),
     QSize(250, 80)));
    selectionFichier->setFrameStyle(QFrame::Panel | QFrame::Sunken);
      selectionFichier->setFont(*baloo);// AJOUT CE
   selectionFichier->setStyleSheet("font-weight:medium; font-size:13pt"); // AJOUT CE
   selectionFichier->setText("Sélectionner votre table de fait");// AJOUT CE
     selectionFichier->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     selectionFichier->setFrameShape(QFrame::HLine);
     selectionFichier->setFrameStyle(QFrame::NoFrame);
    
    m_button = new QPushButton("", this);
    m_button->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_charger_fichier.png);} ");
    m_button->setMaximumSize(150, 40);
    m_button->setMinimumSize(150, 40);
    connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
    
    
    chargerLayout->setAlignment(Qt::AlignTop| Qt::AlignCenter);
    chargerLayout->addWidget(chiffre1,0,0, Qt::AlignTop| Qt::AlignCenter);
    chargerLayout->addWidget(selectionFichier,1,0,Qt::AlignCenter| Qt::AlignCenter);
    chargerLayout->addWidget(m_button,2,0,Qt::AlignTop| Qt::AlignCenter);
    
    chargerBack = new QLabel(this);
    chargerBack->setGeometry(QRect(QPoint(100, 30),
     QSize(300, 300)));
    chargerBack->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    chargerBack->setText("");
     QPixmap pixmapChargerBack("../ui_resources/conteneur_blanc.png");
     chargerBack->setPixmap(pixmapChargerBack.scaled(chargerBack->width(),chargerBack->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     chargerBack->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
     chargerBack->setFrameShape(QFrame::HLine);
     chargerBack->setFrameStyle(QFrame::NoFrame);
}


void MainWindow::initRequeteLayout() {
     requeteLayout = new QGridLayout();
     
     QLabel* chiffre2 = new QLabel(this);
     chiffre2->setGeometry(QRect(QPoint(0,0),
      QSize(50, 50)));
     chiffre2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
      QPixmap pixmapChiffre2("../ui_resources/chiffre_2.png");
      chiffre2->setPixmap(pixmapChiffre2.scaled(chiffre2->width(),chiffre2->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
      chiffre2->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      chiffre2->setFrameShape(QFrame::HLine);
      chiffre2->setFrameStyle(QFrame::NoFrame);
     
     QLabel* champsRequetes = new QLabel(this);
     champsRequetes->setGeometry(QRect(QPoint(0,0),
      QSize(100, 50)));
     champsRequetes->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     champsRequetes->setFont(*baloo);// AJOUT CE
     champsRequetes->setStyleSheet("font-weight:medium; font-size:13pt"); // AJOUT CE
     champsRequetes->setText("Les champs");// AJOUT CE
      champsRequetes->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      champsRequetes->setFrameShape(QFrame::HLine);
      champsRequetes->setFrameStyle(QFrame::NoFrame);
     
     QLabel* fonctionAggregation = new QLabel(this);
     fonctionAggregation->setGeometry(QRect(QPoint(0,0),
      QSize(100, 50)));
     fonctionAggregation->setFrameStyle(QFrame::Panel | QFrame::Sunken);
      fonctionAggregation->setFont(*baloo);// AJOUT CE
   fonctionAggregation->setStyleSheet("font-weight:medium; font-size:13pt"); // AJOUT CE
   fonctionAggregation->setText("Opération");// AJOUT CE
      fonctionAggregation->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      fonctionAggregation->setFrameShape(QFrame::HLine);
      fonctionAggregation->setFrameStyle(QFrame::NoFrame);
     
     champsRequetesComboBox =  new QComboBox(this);
     champsRequetesComboBox->setGeometry(QRect(QPoint(650, 100),
     QSize(100, 50)));
     
     fonctionAggregationComboBox = new QComboBox(this);
     fonctionAggregationComboBox->addItem("Somme");
     fonctionAggregationComboBox->addItem("Max");
     champsRequetesComboBox->setGeometry(QRect(QPoint(650, 100),
     QSize(100, 50)));
    
    listeChampsRetenu = new QTextEdit();
    listeChampsRetenu->setMinimumHeight(20);
    listeChampsRetenu->setMaximumHeight(60);
    listeChampsRetenu->setMinimumWidth(20);
    listeChampsRetenu->setMaximumWidth(120);
    
    validerRequete = new QPushButton("", this);
    validerRequete->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_OK.png); } ");
    validerRequete->setMaximumSize(70, 30);//MODIF CE
    validerRequete->setMinimumSize(70, 30);//MODIF CE
    connect(validerRequete, SIGNAL (released()), this, SLOT (request()));
    
    effacerChamps = new QPushButton("", this);
    effacerChamps->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_effacer.png); } ");
    effacerChamps->setMaximumSize(70, 30);//MODIF CE
    effacerChamps->setMinimumSize(70, 30);//MODIF CE
    connect(effacerChamps, SIGNAL (released()), this, SLOT (effacerListeChamps()));

    
     
     
    requeteLayout->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(chiffre2,0,0,2,0, Qt::AlignTop| Qt::AlignCenter);
    requeteLayout->addWidget(champsRequetes,1,0, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(champsRequetesComboBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(fonctionAggregation,2,0, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(fonctionAggregationComboBox,2,1, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
    requeteLayout->addWidget(validerRequete,3,1, Qt::AlignTop| Qt::AlignCenter);
    requeteLayout->addWidget(effacerChamps,4,1, Qt::AlignTop| Qt::AlignCenter);
    
    
    requeteBack = new QLabel(this);
    requeteBack->setGeometry(QRect(QPoint(100, 30),
     QSize(300, 300)));
    requeteBack->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    requeteBack->setText("");
     QPixmap pixmaRequete("../ui_resources/conteneur_blanc.png");
     requeteBack->setPixmap(pixmaRequete.scaled(requeteBack->width(),requeteBack->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     requeteBack->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
     requeteBack->setFrameShape(QFrame::HLine);
     requeteBack->setFrameStyle(QFrame::NoFrame);
}


void MainWindow::nbMterialisationLayout() {
    nbMaterialiserBack = new QLabel(this);
    nbMaterialiserBack->setGeometry(QRect(QPoint(100, 30),
     QSize(300, 300)));
    nbMaterialiserBack->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    nbMaterialiserBack->setText("");
     QPixmap pixmaRequete("../ui_resources/conteneur_blanc.png");
     nbMaterialiserBack->setPixmap(pixmaRequete.scaled(nbMaterialiserBack->width(),nbMaterialiserBack->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     nbMaterialiserBack->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
     nbMaterialiserBack->setFrameShape(QFrame::HLine);
     nbMaterialiserBack->setFrameStyle(QFrame::NoFrame);
    
    QLabel* chiffre3 = new QLabel(this);
    chiffre3->setGeometry(QRect(QPoint(0,0),
     QSize(50, 50)));
    chiffre3->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     QPixmap pixmapChiffre2("../ui_resources/chiffre_3.png");
     chiffre3->setPixmap(pixmapChiffre2.scaled(chiffre3->width(),chiffre3->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     chiffre3->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     chiffre3->setFrameShape(QFrame::HLine);
     chiffre3->setFrameStyle(QFrame::NoFrame);
       
    QLabel* nbMaterialiser = new QLabel(this);
    nbMaterialiser->setGeometry(QRect(QPoint(50,0),
     QSize(120, 50)));
    nbMaterialiser->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    nbMaterialiser->setFont(*baloo);// AJOUT CE
     nbMaterialiser->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 10px"); // AJOUT CE
     nbMaterialiser->setText("Nombre de requêtes" "\n" "à matérialiser");// AJOUT CE
     nbMaterialiser->setAlignment(Qt::AlignTop | Qt::AlignLeft);

     nbMaterialiser->setFrameShape(QFrame::HLine);
     nbMaterialiser->setFrameStyle(QFrame::NoFrame);
     nbRequetesAMaterialiserBox =  new QComboBox(this);
    
    QLabel* uniteMemoire = new QLabel(this);
    uniteMemoire->setGeometry(QRect(QPoint(0,0),
     QSize(100, 50)));
    uniteMemoire->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     uniteMemoire->setFont(*baloo);// AJOUT CE
     uniteMemoire->setStyleSheet("font-weight:medium; font-size:13pt"); // AJOUT CE
     uniteMemoire->setText("Unité de mémoire");// AJOUT CE

     uniteMemoire->setAlignment(Qt::AlignTop | Qt::AlignLeft);
     uniteMemoire->setFrameShape(QFrame::HLine);
     uniteMemoire->setFrameStyle(QFrame::NoFrame);
    
    textMemoire = new QLabel(this);
    textMemoire->setGeometry(QRect(QPoint(0,0),
     QSize(100, 50)));
    textMemoire->setText("0");
    textMemoire->setFrameShape(QFrame::HLine);
    textMemoire->setFrameStyle(QFrame::NoFrame);
    textMemoire->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     textMemoire->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     nbRequetesAMaterialiserBox =  new QComboBox(this);
    
    nbRequetesAMaterialiserButton = new QPushButton(this);
    nbRequetesAMaterialiserButton->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_OK.png); } ");
    nbRequetesAMaterialiserButton->setMaximumSize(70, 30); //MODIF CE
    nbRequetesAMaterialiserButton->setMinimumSize(70, 30); //MODIF CE
    connect(nbRequetesAMaterialiserButton, SIGNAL (released()), this, SLOT (calculRequetesAMateriliser()));
    
    nbMaterialiserLayout = new QGridLayout();
    nbMaterialiserLayout->setAlignment(Qt::AlignTop| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(chiffre3,0,0,2,0, Qt::AlignTop| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(nbMaterialiser,1,0, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(uniteMemoire,2,0, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(textMemoire,2,1, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserButton,3,1, Qt::AlignCenter| Qt::AlignCenter);

}


void MainWindow::initExporterLayout() {
    
    exporterBack = new QLabel(this);
    exporterBack->setGeometry(QRect(QPoint(100, 30),
     QSize(300, 300)));
    exporterBack->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    exporterBack->setText("");
     QPixmap pixmaRequete("../ui_resources/conteneur_blanc.png");
     exporterBack->setPixmap(pixmaRequete.scaled(exporterBack->width(),exporterBack->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     exporterBack->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
     exporterBack->setFrameShape(QFrame::HLine);
     exporterBack->setFrameStyle(QFrame::NoFrame);
    

    QLabel* chiffre3 = new QLabel(this);
    chiffre3->setGeometry(QRect(QPoint(0,0),
     QSize(50, 50)));
    chiffre3->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     QPixmap pixmapChiffre2("../ui_resources/chiffre_4.png");
     chiffre3->setPixmap(pixmapChiffre2.scaled(chiffre3->width(),chiffre3->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     chiffre3->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     chiffre3->setFrameShape(QFrame::HLine);
     chiffre3->setFrameStyle(QFrame::NoFrame);  

    QLabel* selectionFichier = new QLabel(this);
    selectionFichier->setGeometry(QRect(QPoint(0,0),
     QSize(250, 80)));
    selectionFichier->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     selectionFichier->setFont(*baloo);// AJOUT CE
     selectionFichier->setStyleSheet("font-weight:medium; font-size:13pt ; padding-left: 8px"); // AJOUT CE
     selectionFichier->setText("Souhaitez-vous exporter la table" "\n"  "de fait générée ?");// AJOUT CE
     selectionFichier->setAlignment(Qt::AlignTop | Qt::AlignLeft);
     selectionFichier->setFrameShape(QFrame::HLine);
     selectionFichier->setFrameStyle(QFrame::NoFrame);
    
    exporterButton = new QPushButton(this);
    exporterButton->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_exporter.png); } ");
    exporterButton->setMaximumSize(100, 40);
    exporterButton->setMinimumSize(100, 40);
    connect(exporterButton, SIGNAL (released()), this, SLOT (exporterButtonLaunch()));
    
    exporterLayout = new QGridLayout();
    exporterLayout->addWidget(chiffre3,0,0, Qt::AlignTop| Qt::AlignCenter);
    exporterLayout->addWidget(selectionFichier,1,0,Qt::AlignCenter| Qt::AlignCenter);
    exporterLayout->addWidget(exporterButton,2,0,Qt::AlignCenter| Qt::AlignCenter);
    
}


void MainWindow::request() {
    if(tableFaitString.size() == 0) {
        msgBox->setText("Commencez par importer une table avant de requeter");
        msgBox->show();
        return;
        
    }
    
    tempsRequete = 0.0;
    clock_t begin = clock();
    
    string request = listeChampsRetenu->toPlainText().toUtf8().constData();
    listeChampsRetenu->setText(listeChampsRetenu->toPlainText().toUtf8().constData());
    requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
    vector<string> requete = split(request,';');
    int operation = 0;
    string fonctionAggregation = fonctionAggregationComboBox->itemText(fonctionAggregationComboBox->currentIndex()).toUtf8().constData();
    if( fonctionAggregation == "Max")
        operation = 1;
    materialiserRequete(requete,taillesRequetes, requetesMaterialise, tableFaitString, map_Sum, map_Max, operation, tableFaitRequete);
    tableFaitRequeteWidget = new QTableWidget();
    if(this->tableFaitRequete.size() > 1000) {
        tableFaitRequeteWidget->setRowCount(1000);
    }
    else tableFaitRequeteWidget->setRowCount(this->tableFaitRequete.size()-1);
    tableFaitRequeteWidget->setColumnCount(this->tableFaitRequete[0].size());
    for(int i = 0; i<tableFaitRequete[0].size(); i++) {
       tableFaitRequeteWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromStdString(tableFaitRequete[0][i])));
    }
    for(int i = 1; i<tableFaitRequete.size(); i++) {
        for(int j = 0; j<tableFaitRequete[0].size(); j++){
            QTableWidgetItem *item = tableFaitRequeteWidget->item(-1, j);
            if(!item) {
                item = new QTableWidgetItem();
                tableFaitRequeteWidget->setItem(i-1, j, item);
            }
            item->setText(QString::fromStdString(tableFaitRequete[i][j]));
        }
    }
    tableFaitRequeteWidget->setGeometry(QRect(QPoint(40, 200),
    QSize(1500, 300)));
    tableFaitRequeteWidget->setMaximumWidth(630);
    tableFaitRequeteWidget->setMinimumWidth(630);
    tableFaitRequeteWidget->setMaximumHeight(410);
    tableFaitRequeteWidget->setMinimumHeight(410);
    tableFaitRequeteWidget->resizeColumnsToContents();
    tableFaitRequeteWidget->resizeRowsToContents();
    
    mainLayout->addWidget(tableFaitRequeteWidget, 4,2,1,2, Qt::AlignTop| Qt::AlignLeft);
    
    clock_t end = clock();
    tempsRequete += (double)(end - begin)/CLOCKS_PER_SEC;
    string optimisation = "Requete optimisée";
    if (requetesMaterialise.size() <= 1) {
        optimisation = "Requete non optimisée";
    }
    tempsReq->setText(QString::fromStdString(to_string(tempsRequete)) + "s" "\n" + QString::fromStdString(optimisation));
    mainLayout->addWidget(tempsReq, 2, 3);
}


vector<string> MainWindow::split(const string &s, char delim) {
  stringstream ss(s);
  string item;
  vector<string> elems;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

void MainWindow::onClickChampsComboBox(int) {
    string selected = champsRequetesComboBox->itemText(champsRequetesComboBox->currentIndex()).toUtf8().constData();
    if(!selected.empty()) {
        listeChampsRetenu->setText(listeChampsRetenu->toPlainText().toUtf8().constData() + QString::fromStdString(selected + ";"));
        requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
    }
}

void MainWindow::effacerListeChamps() {
    listeChampsRetenu->setText("");
    requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
}

void MainWindow::tailleMaxVector(int) {
    int selected = stoi(nbRequetesAMaterialiserBox->itemText(nbRequetesAMaterialiserBox->currentIndex()).toUtf8().constData());
    espaceMemoire = espaceMemoirePrevu(taillesRequetes, selected);
    textMemoire->setGeometry(QRect(QPoint(0,0),
     QSize(100, 50)));
    textMemoire->setText(QString::fromStdString(to_string(espaceMemoire)));
    textMemoire->setFrameShape(QFrame::HLine);
    textMemoire->setFrameStyle(QFrame::NoFrame);
    textMemoire->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     textMemoire->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    
    nbMaterialiserLayout->addWidget(textMemoire,2,1, Qt::AlignCenter| Qt::AlignCenter);
}

void MainWindow::runCaculRequete() {
    nbAMateriliser = stoi(nbRequetesAMaterialiserBox->itemText(nbRequetesAMaterialiserBox->currentIndex()).toUtf8().constData());
    emit endCalculRequete(10);
    requetesMaterialise = calculBeneficeTotal(taillesRequetes, nbAMateriliser);
    emit endCalculRequete(50);
    espaceMemoireReel = espaceMemoireUtilise(taillesRequetes, requetesMaterialise);
    emit endCalculRequete(55);
    stockerRequete(requetesMaterialise, tableFaitString, map_Sum, map_Max);
    emit endCalculRequete(100);
}


void MainWindow::displayPopupEndCalculRequete(int value){
    bar->setValue(value);
    if(value == 100) {
        string message = "Les requetes sont préchargées ! <br><br>";
        if(nbAMateriliser > requetesMaterialise.size()){
            message +=  to_string(requetesMaterialise.size()) + " matérialisations suffisent à optimiser les requêtes utilisant " + to_string(espaceMemoireReel) + " d'espace mémoire";
        }
        msgBox->setText(QString::fromStdString(message));
        msgBox->show();
    }
}

void MainWindow::runChargementFichier() {
    string filePath = dirPath.toUtf8().constData();
    tableFaitString = chargerFichiers(filePath);
    emit endChargementFichier(10);
    tableFait = conversion(tableFaitString);
    emit endChargementFichier(50);
    taillesRequetes = toutes_les_tailles(tableFait);
    emit endChargementFichier(100);
}

void MainWindow::displayPopupEndChargementFichier(int value) {
    bar->setValue(value);
    if(value == 100) {
        initTableFaitView();
        initTableTailleRequetesWidget();
        msgBox->setText(QString::fromStdString("Ouverture et chargement du fichier :  <br><br>") + dirPath.toUtf8().constData() + QString::fromStdString(" termninée !"));
        msgBox->show();
    }
}

void MainWindow::exporterButtonLaunch() {
    if(tableFaitString.size() == 0 || tableFaitRequete.size() == 0) {
        msgBox->setText("Commencez par faire une requêtes");
        msgBox->show();
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "../resources/table.csv", tr("Images (*.csv)"));
    if(filename.isEmpty()) {
        return;
    }
    QFileInfo f( filename );
    saveDirPath = f.filePath(); // Path vers le fichier
    saveFileName = f.fileName();
    bar = new QProgressDialog();
    bar->setAutoClose(true);
    bar->setCancelButtonText(QString());
    bar->setRange(0,100);
    bar->setValue(0);
    mainLayout->addWidget(bar,0,3, Qt::AlignTop| Qt::AlignRight);
    connect(this, SIGNAL(endExportFichier(int)), this, SLOT(displayPopupExporter(int)), Qt::BlockingQueuedConnection);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::exporter );
}

void MainWindow::exporter() {
    emit endExportFichier(0);
    string path = saveDirPath.toUtf8().constData();
    exportFichier(tableFaitRequete, path);
    emit endExportFichier(100);
}

void MainWindow::displayPopupExporter(int value) {
    bar->setValue(value);
    if(value == 100) {
        msgBox->setText(QString::fromStdString("Fichier enregistré!"));
        msgBox->show();
    }
}




