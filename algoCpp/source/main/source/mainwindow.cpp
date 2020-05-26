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
#include <QTextCursor>
#include <QMenu>
#include <QToolBar>
#include <QIcon>
#include <QToolButton>
#include <QStatusBar>
#include <QTextBrowser>
#include <QFile>
#include <QDesktopWidget>
#include <QApplication>




using namespace std;

//Constructeur de la fenêtre et de tous les objets et widgets la composant au lancement
MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
    scaleWidthRatio = (double)QApplication::desktop()->width() / baseWidth;// Récupération de la largeur et hauteur de l'ordinateur
    scaleHeigthRatio = (double)QApplication::desktop()->height() / baseHeigth;// Afin d'adapter les tailles de layout à la résolution de l'écran
    
    long id = QFontDatabase::addApplicationFont("../ui_resources/Baloo-Regular-webfont.ttf");// Téléchargement de la police Baloo-regular
    if(id!=-1) {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);// Attribution de la police à l'attribut "baloo"
        baloo = new QFont(family);
    }
    else baloo = new QFont(QString::fromStdString("Baloo"));// Si échec de l'installation de la police on apelle directement la police
    // de l'ordinateur (LES PC MACS POSSEDENT DEJA CETTE POLICE)
    
    //Chargement de la bar pour les infos et tutoriels
    QToolBar* toolbar = new QToolBar;
    toolbar->setStyleSheet("border: none; background-color: transparent;");
    toolbar->setGeometry(QRect(QPoint(100, 30),
    QSize(30 * scaleWidthRatio, 20 * scaleHeigthRatio)));
   
    QToolButton* helpButton = new QToolButton();
    helpButton->setStyleSheet("QToolButton{border-image:url(../ui_resources/help.png); max-height: 25px; max-width: 25px;} QToolButton:hover{border-image:url(../ui_resources/help_hover.png); max-height: 25px; max-width: 25px;} QToolButton:pressed{border-image:url(../ui_resources/help_hover.png);max-height: 15px; max-width: 15px;}");    // Attribution d'éléments de style CSS au bouton CSS                                                                 
    QToolButton* infoButton = new QToolButton();
    infoButton->setStyleSheet("QToolButton{border-image:url(../ui_resources/info.png); max-height: 25px; max-width: 25px;} QToolButton:hover{border-image:url(../ui_resources/info_hover.png);max-height: 25px; max-width: 25px;} QToolButton:pressed{border-image:url(../ui_resources/info_hover.png);max-height: 15px; max-width: 15px;} ");       // Attribution d'éléments type CSS au bouton
    
    toolbar->addWidget(helpButton);
    toolbar->addWidget(infoButton);
    connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
    connect(infoButton, SIGNAL(clicked()), this, SLOT(info()));
    
    statusBar = new QStatusBar();
    statusBar->addPermanentWidget(toolbar);    
    
    //Initialisation bar de progression
    bar = new QProgressDialog();
    barRequest = new QProgressDialog();
    barRequest->setVisible(false);
    barExport = new QProgressDialog();
    barExport->setVisible(false);
    barMat = new QProgressDialog();
    barMat->setVisible(false);
    
    barMat->setAutoClose(true);
    barMat->setCancelButtonText(QString());
    barMat->setRange(0,100);
    barMat->setValue(100);
    
    bar->setAutoClose(true);
    bar->setCancelButtonText(QString());
    bar->setRange(0,100);
    bar->setValue(100);
    
    barExport->setAutoClose(true);
    barExport->setCancelButtonText(QString());
    barExport->setRange(0,100);
    barExport->setValue(100);
    
    barRequest->setAutoClose(true);
    barRequest->setCancelButtonText(QString());
    barRequest->setRange(0,100);
    barRequest->setValue(100);
    
    requetesMaterialise.resize(0);
    
    //Chargement de la vue de chargement de fichier
    initiChargerLayout();
    //Chargement de la vue requete de fichier
    initRequeteLayout();
    //Chargement de vue de matérialisation
    nbMterialisationLayout();
    //Chargement de la vue d'exportation
    initExporterLayout();

    //Chargement de l'objet de temps requête
   tempsReq = new QLabel(this);
   tempsReq->setGeometry(QRect(QPoint(100, 30),
    QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
   tempsReq->setFrameStyle(QFrame::Panel | QFrame::Sunken);

   tempsReq->setFont(*baloo);                                               // Attribution de la police "baloo" au label
   tempsReq->setStyleSheet("font-weight:medium; font-size:14pt");           // Attribution d'éléments de style CSS au texte
   tempsReq->setText(QString::fromStdString(to_string(tempsRequete)) + "s");
    tempsReq->setAlignment(Qt::AlignTop | Qt::AlignRight);
    tempsReq->setFrameShape(QFrame::HLine);
    tempsReq->setFrameStyle(QFrame::NoFrame);
    
    //Chargement de la table de fait widget
    tailleTableFait = new QLabel();
    tailleTableFait->setGeometry(QRect(QPoint(100, 30),
     QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    tailleTableFait->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    tailleTableFait->setFont(*baloo);  					    // Attribution de la police "baloo" au label
    tailleTableFait->setStyleSheet("font-weight:medium; font-size:13pt");   // Attribution d'éléments de style CSS au texte
    tailleTableFait->setText("0 lignes");                                   // Attribution d'un texte au label
     tailleTableFait->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     tailleTableFait->setFrameShape(QFrame::HLine);
     tailleTableFait->setFrameStyle(QFrame::NoFrame);
    
    tailleResultatRequete = new QLabel();
    tailleResultatRequete->setGeometry(QRect(QPoint(100, 30),
     QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    tailleResultatRequete->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    tailleResultatRequete->setFont(*baloo);				          // Attribution de la police "baloo" au label
    tailleResultatRequete->setStyleSheet("font-weight:medium; font-size:13pt");   // Attribution d'éléments de style CSS au texte
    tailleResultatRequete->setText("0 lignes");                                   // Attribution d'un texte au label         
     tailleResultatRequete->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     tailleResultatRequete->setFrameShape(QFrame::HLine);
     tailleResultatRequete->setFrameStyle(QFrame::NoFrame);
    
   title = new QLabel(this);
   title->setGeometry(QRect(QPoint(100, 30),
    QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
   title->setFrameStyle(QFrame::Panel | QFrame::Sunken);

   title->setFont(*baloo);                                                  // Attribution de la police "baloo" au label
   title->setStyleSheet("font-weight:medium; font-size:28pt");              // Attribution d'éléments de style CSS au texte
   title->setText("QueryOptimizer");                                        // Attribution d'un texte au label   
    title->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    title->setFrameShape(QFrame::HLine);
    title->setFrameStyle(QFrame::NoFrame);
    
    titleTableFait = new QLabel(this);
    titleTableFait->setGeometry(QRect(QPoint(100, 30),
     QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    titleTableFait->setFrameStyle(QFrame::Panel | QFrame::Sunken);
						
   titleTableFait->setFont(*baloo);							 // Attribution de la police "baloo" au label
   titleTableFait->setStyleSheet("font-weight:medium; font-size:16pt;color:grey");       // Attribution d'éléments de style CSS au texte
   titleTableFait->setText("Table de faits - Originale");                                // Attribution d'un texte au label  
     titleTableFait->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     titleTableFait->setFrameShape(QFrame::HLine);
     titleTableFait->setFrameStyle(QFrame::NoFrame);
    
    titleTableGeneree = new QLabel(this);
    titleTableGeneree->setGeometry(QRect(QPoint(100, 30),
     QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    titleTableGeneree->setFrameStyle(QFrame::Panel | QFrame::Sunken);

   titleTableGeneree->setFont(*baloo);							 // Attribution de la police "baloo" au label
   titleTableGeneree->setStyleSheet("font-weight:medium; font-size:16pt;color:grey");    // Attribution d'éléments de style CSS au texte
   titleTableGeneree->setText("Résultat requête");				         // Attribution d'un texte au label  
     titleTableGeneree->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     titleTableGeneree->setFrameShape(QFrame::HLine);
     titleTableGeneree->setFrameStyle(QFrame::NoFrame);
    
    
    //Layout principal en grille
    mainLayout = new QGridLayout();
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->setRowMinimumHeight(4, 380 * scaleHeigthRatio);
    mainLayout->setRowMinimumHeight(0, 40 * scaleHeigthRatio);
    mainLayout->setVerticalSpacing(20 * scaleHeigthRatio);
    mainLayout->setHorizontalSpacing(60 * scaleWidthRatio);
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
    mainLayout->addWidget(tailleResultatRequete, 5, 2);
    mainLayout->addWidget(tailleTableFait, 5, 0);

    QWidget* contentWidget = new QWidget();
    contentWidget->setLayout(mainLayout);

    mainScrollArea = new QScrollArea();
    mainScrollArea->setWidget(contentWidget);
    mainScrollArea->resize(2000 * scaleWidthRatio ,4000 * scaleHeigthRatio);

    setCentralWidget(mainScrollArea);
    msgBox = new QMessageBox();
    msgBox->setWindowTitle("QueryOptimizer");                            // Attribution d'un titre à la fenêtre du msgBox
    tableFaitWidget = new QTableWidget();
    tableFaitWidget->setMaximumWidth(650 * scaleWidthRatio);
    tableFaitWidget->setMinimumWidth(650 * scaleWidthRatio);
    tableFaitWidget->setMaximumHeight(370 * scaleHeigthRatio);
    tableFaitWidget->setMinimumHeight(370 * scaleHeigthRatio);
    mainLayout->addWidget(tableFaitWidget, 4,0,1,2, Qt::AlignTop| Qt::AlignLeft);
    
    
    tableFaitRequeteWidget = new QTableWidget();
    tableFaitRequeteWidget->setMaximumWidth(650 * scaleWidthRatio);
    tableFaitRequeteWidget->setMinimumWidth(650 * scaleWidthRatio);
    tableFaitRequeteWidget->setMaximumHeight(370 * scaleHeigthRatio);
    tableFaitRequeteWidget->setMinimumHeight(370 * scaleHeigthRatio);
    mainLayout->addWidget(tableFaitRequeteWidget, 4,2,1,2, Qt::AlignTop| Qt::AlignLeft);
    
    mainLayout->addWidget(barMat,0,3, Qt::AlignCenter| Qt::AlignCenter);
    mainLayout->addWidget(bar,0,3, Qt::AlignCenter| Qt::AlignCenter);
    mainLayout->addWidget(barRequest,0,3, Qt::AlignCenter| Qt::AlignCenter);
    mainLayout->addWidget(barExport,0,3, Qt::AlignCenter| Qt::AlignCenter);
    this->setStatusBar(statusBar);
}

void MainWindow::handleButton()
{
    //Gestion du bouton de chargement
    QString file = QFileDialog::getOpenFileName(this, tr("Charger une table de fait ..."),"../resources",tr("BDD (*.csv)"));
    if(file.isEmpty()) {
        return;
    }
    bar->setValue(0);
    bar->show();
    QFileInfo fileInfo(file);
    dirPath = fileInfo.filePath(); // Path vers le fichier
    fileName = fileInfo.fileName();
    string filePath = dirPath.toUtf8().constData();
    connect(this, SIGNAL(endChargementFichier(long)), this, SLOT(displayPopupEndChargementFichier(long)), Qt::BlockingQueuedConnection);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::runChargementFichier );
}

void MainWindow::initTableFaitView(){
    //Chargement de la table fait
    requetesMaterialise.clear();
    map_Max.clear();
    map_Sum.clear();
    espaceMemoireReel = 0;
    memoireReelleVal->setText("0");
    reqMatVal->setText("0");
    if(this->tableFaitString.size() > 1000) {
        tableFaitWidget->setRowCount(1000);
    }
    else tableFaitWidget->setRowCount(this->tableFaitString.size()-1);
    tableFaitWidget->setColumnCount(this->tableFaitString[0].size());
    for(long i = 0; i<tableFaitString[0].size(); i++) {
       tableFaitWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromStdString(tableFaitString[0][i])));
    }
    for(long i = 1; i<tableFaitString.size(); i++) {
        for(long j = 0; j<tableFaitString[0].size(); j++){
            QTableWidgetItem *item = tableFaitWidget->item(-1, j);
            if(!item) {
                item = new QTableWidgetItem();
                tableFaitWidget->setItem(i-1, j, item);
            }
            item->setText(QString::fromStdString(tableFaitString[i][j]));
        }
    }
    tableFaitWidget->resizeColumnsToContents();
    tableFaitWidget->resizeRowsToContents();
    
    mainLayout->addWidget(tableFaitWidget, 4,0,1,2, Qt::AlignTop| Qt::AlignLeft);
    delete champsRequetesComboBox;
    champsRequetesComboBox =  new QComboBox(this);
    champsRequetesComboBox->addItem("");
    for(long i = 0; i< tableFaitString[0].size() - 1; i++) {
        champsRequetesComboBox->addItem(QString::fromStdString(tableFaitString[0][i]));
    }
    champsRequetesComboBox->setGeometry(QRect(QPoint(650, 100),
    QSize(100 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    connect(champsRequetesComboBox,SIGNAL (currentIndexChanged(int)),this,SLOT(onClickChampsComboBox(int)));
    requeteLayout->addWidget(champsRequetesComboBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
}

void MainWindow::initTableTailleRequetesWidget(){
    requetesMaterialise.resize(1);
    requetesMaterialise[0] = taillesRequetes.size()-1;
    nbRequetesAMaterialiserBox->setText("0");
    nbRequetesAMaterialiserLabel->setText("Nb requêtes à matérialiser" "\n" "Max : " + QString::fromStdString(to_string(taillesRequetes.size()-1)));  // Attribution d'un texte au label avec un saut de ligne "\n"
    connect(nbRequetesAMaterialiserBox,SIGNAL (textChanged(QString)),this,SLOT(tailleMaxVector(QString)));
    nbRequetesAMaterialiserBox->setGeometry(QRect(QPoint(650, 100),
    QSize(100 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
}


void MainWindow::calculRequetesAMateriliser() {
    //Concurrent thread pour matérialiser les requêtes et pas bloquer le thread principal de la fenêtre graphique
    if(tableFaitString.size() == 0) {
        msgBox->setText("Commencez par importer une table avant d'optimiser");
        msgBox->show();
        return;
    }
    barMat->setValue(0);
    barMat->show();
    connect(this, SIGNAL(endCalculRequete(long)), this, SLOT(displayPopupEndCalculRequete(long)), Qt::BlockingQueuedConnection);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::runCaculRequete );
}

void MainWindow::initiChargerLayout() {
    chargerLayout = new QGridLayout();
    
    QLabel* chiffre1 = new QLabel(this);
    chiffre1->setGeometry(QRect(QPoint(0,0),
     QSize(150 * scaleWidthRatio, 50 *scaleHeigthRatio)));
    chiffre1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QPixmap pixmapChiffre1("../ui_resources/import_fichier.png");
    chiffre1->setPixmap(pixmapChiffre1.scaled(chiffre1->width(),chiffre1->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    chiffre1->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    chiffre1->setFrameShape(QFrame::HLine);
    chiffre1->setFrameStyle(QFrame::NoFrame);

    QLabel* selectionFichier = new QLabel(this);
    selectionFichier->setGeometry(QRect(QPoint(0,0),
     QSize(250 * scaleWidthRatio, 80 * scaleHeigthRatio)));
    selectionFichier->setFrameStyle(QFrame::Panel | QFrame::Sunken);
      selectionFichier->setFont(*baloo);
   selectionFichier->setStyleSheet("font-weight:medium; font-size:13pt"); 
   selectionFichier->setText("Sélectionner votre table de fait");
     selectionFichier->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     selectionFichier->setFrameShape(QFrame::HLine);
     selectionFichier->setFrameStyle(QFrame::NoFrame);
    
    m_button = new QPushButton("", this);
    m_button->setFont(*baloo); 									// Attribution de la police "baloo" au label
    m_button->setStyleSheet("QPushButton{background-color:#191970; border-radius:10px;font-weight:medium; font-size:13pt;color:white}QPushButton:hover{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:13pt;color:white} QPushButton:pressed{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:10pt;color:white}"); // Attribution d'éléments de style CSS au bouton
    m_button->setText("Charger le fichier");							// Attribution d'un texte au label  
    m_button->setMaximumSize(150 * scaleWidthRatio, 40 * scaleHeigthRatio);
    m_button->setMinimumSize(150 * scaleWidthRatio, 40 * scaleHeigthRatio);
    connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
    
    
    chargerLayout->setAlignment(Qt::AlignTop| Qt::AlignCenter);
    chargerLayout->addWidget(chiffre1,0,0, Qt::AlignTop| Qt::AlignCenter);
    chargerLayout->addWidget(selectionFichier,1,0,Qt::AlignCenter| Qt::AlignCenter);
    chargerLayout->addWidget(m_button,2,0,Qt::AlignCenter| Qt::AlignCenter);
    
    chargerBack = new QLabel(this);
    chargerBack->setGeometry(QRect(QPoint(100, 30),
     QSize(300 * scaleWidthRatio, 300 * scaleHeigthRatio)));
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
      QSize(150 * scaleWidthRatio, 50 * scaleHeigthRatio)));
     chiffre2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
      QPixmap pixmapChiffre2("../ui_resources/requetage.png");
      chiffre2->setPixmap(pixmapChiffre2.scaled(chiffre2->width(),chiffre2->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
      chiffre2->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      chiffre2->setFrameShape(QFrame::HLine);
      chiffre2->setFrameStyle(QFrame::NoFrame);
     
     QLabel* champsRequetes = new QLabel(this);
     champsRequetes->setGeometry(QRect(QPoint(0,0),
      QSize(100 * scaleWidthRatio, 50 * scaleHeigthRatio)));
     champsRequetes->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		
     champsRequetes->setFont(*baloo);									// Attribution de la police "baloo" au label
     champsRequetes->setStyleSheet("font-weight:medium; font-size:13pt"); 				// Attribution d'éléments de style CSS au texte
     champsRequetes->setText("Choisissez des" "\n"  "dimensions");					// Attribution d'un texte au label avec un saut de ligne "\n"
      champsRequetes->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      champsRequetes->setFrameShape(QFrame::HLine);
      champsRequetes->setFrameStyle(QFrame::NoFrame);
     
     QLabel* fonctionAggregation = new QLabel(this);
     fonctionAggregation->setGeometry(QRect(QPoint(0,0),
      QSize(100 * scaleWidthRatio, 50 * scaleHeigthRatio)));
     fonctionAggregation->setFrameStyle(QFrame::Panel | QFrame::Sunken);

      fonctionAggregation->setFont(*baloo);								// Attribution de la police "baloo" au label
   fonctionAggregation->setStyleSheet("font-weight:medium; font-size:13pt"); 				// Attribution d'éléments de style CSS au texte
   fonctionAggregation->setText("Choisissez une" "\n" "opération");					// Attribution d'un texte au label avec un saut de ligne "\n"
      fonctionAggregation->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      fonctionAggregation->setFrameShape(QFrame::HLine);
      fonctionAggregation->setFrameStyle(QFrame::NoFrame);
     
     champsRequetesComboBox =  new QComboBox(this);
     champsRequetesComboBox->setGeometry(QRect(QPoint(650, 100),
     QSize(100 * scaleWidthRatio, 50 * scaleHeigthRatio)));
     
     fonctionAggregationComboBox = new QComboBox(this);
     fonctionAggregationComboBox->addItem("Somme");
     fonctionAggregationComboBox->addItem("Max");
     champsRequetesComboBox->setGeometry(QRect(QPoint(650, 100),
     QSize(100 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    
    listeChampsRetenu = new QTextEdit();
    listeChampsRetenu->setMinimumHeight(20 * scaleHeigthRatio);
    listeChampsRetenu->setMaximumHeight(60 * scaleHeigthRatio);
    listeChampsRetenu->setMinimumWidth(20 * scaleWidthRatio);
    listeChampsRetenu->setMaximumWidth(120 * scaleWidthRatio);
    
    validerRequete = new QPushButton("", this);
    validerRequete->setFont(*baloo);									// Attribution de la police "baloo" au label
    validerRequete->setStyleSheet("QPushButton{background-color:#191970; border-radius:10px;font-weight:medium; font-size:13pt;color:white}QPushButton:hover{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:13pt;color:white} QPushButton:pressed{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:10pt;color:white}"); // Attribution d'éléments de style CSS au texte
    validerRequete->setText("OK");									// Attribution d'un texte au label
    validerRequete->setMaximumSize(70 * scaleWidthRatio, 30 * scaleHeigthRatio);
    validerRequete->setMinimumSize(70 * scaleWidthRatio, 30 * scaleHeigthRatio);
    connect(validerRequete, SIGNAL (released()), this, SLOT (request()));
    
    effacerChamps = new QPushButton("", this);
    effacerChamps->setFont(*baloo);									// Attribution de la police "baloo" au label
    effacerChamps->setStyleSheet("QPushButton{background-color:#191970; border-radius:10px;font-weight:medium; font-size:13pt;color:white}QPushButton:hover{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:13pt;color:white} QPushButton:pressed{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:10pt;color:white}");// Attribution d'éléments de style CSS au texte
    effacerChamps->setText("Effacer");									// Attribution d'un texte au label
    effacerChamps->setMaximumSize(70 * scaleWidthRatio, 30 * scaleHeigthRatio);
    effacerChamps->setMinimumSize(70 * scaleWidthRatio, 30 * scaleHeigthRatio);
    connect(effacerChamps, SIGNAL (released()), this, SLOT (effacerListeChamps()));

    
     
     
    requeteLayout->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(chiffre2,0,0,2,0, Qt::AlignTop| Qt::AlignCenter);
    requeteLayout->addWidget(champsRequetes,1,0, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(champsRequetesComboBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(fonctionAggregation,2,0, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(fonctionAggregationComboBox,2,1, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
    requeteLayout->addWidget(validerRequete,4,1, Qt::AlignCenter| Qt::AlignCenter);
    requeteLayout->addWidget(effacerChamps,3,1, Qt::AlignCenter| Qt::AlignCenter);
    
    
    requeteBack = new QLabel(this);
    requeteBack->setGeometry(QRect(QPoint(100, 30),
     QSize(300 * scaleWidthRatio, 300 * scaleHeigthRatio)));
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
     QSize(300 * scaleWidthRatio, 300 * scaleHeigthRatio)));
    nbMaterialiserBack->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    nbMaterialiserBack->setText("");
     QPixmap pixmaRequete("../ui_resources/conteneur_blanc.png");
     nbMaterialiserBack->setPixmap(pixmaRequete.scaled(nbMaterialiserBack->width(),nbMaterialiserBack->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     nbMaterialiserBack->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
     nbMaterialiserBack->setFrameShape(QFrame::HLine);
     nbMaterialiserBack->setFrameStyle(QFrame::NoFrame);
    
    QLabel* chiffre3 = new QLabel(this);
    chiffre3->setGeometry(QRect(QPoint(0,0),
     QSize(150 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    chiffre3->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     QPixmap pixmapChiffre2("../ui_resources/optimisation.png");
     chiffre3->setPixmap(pixmapChiffre2.scaled(chiffre3->width(),chiffre3->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     chiffre3->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     chiffre3->setFrameShape(QFrame::HLine);
     chiffre3->setFrameStyle(QFrame::NoFrame);
       
    nbRequetesAMaterialiserLabel = new QLabel(this);
    nbRequetesAMaterialiserLabel->setGeometry(QRect(QPoint(0,0),
     QSize(120 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    nbRequetesAMaterialiserLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    nbRequetesAMaterialiserLabel->setFont(*baloo);								// Attribution de la police "baloo" au label
     nbRequetesAMaterialiserLabel->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 14px");     // Attribution d'éléments de style CSS au texte
     nbRequetesAMaterialiserLabel->setText("Nb requêtes à matérialiser" "\n" "Max : 0");			// Attribution d'un texte au label avec saut de ligne "\n"
     nbRequetesAMaterialiserLabel->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

     nbRequetesAMaterialiserLabel->setFrameShape(QFrame::HLine);
     nbRequetesAMaterialiserLabel->setFrameStyle(QFrame::NoFrame);
    
    memoireReelle = new QLabel(this);
    memoireReelle->setGeometry(QRect(QPoint(0,0),
     QSize(200 * scaleWidthRatio, 30 * scaleHeigthRatio)));

    memoireReelle->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		
    memoireReelle->setFont(*baloo);						 				// Attribution de la police "baloo" au label				
     memoireReelle->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 21px");     		 // Attribution d'éléments de style CSS au texte
    memoireReelle->setText("Unités de mémoire utilisées");							// Attribution d'un texte au label
     memoireReelle->setAlignment(Qt::AlignCenter | Qt::AlignLeft);


     memoireReelle->setFrameShape(QFrame::HLine);
     memoireReelle->setFrameStyle(QFrame::NoFrame);
    
    reqMat = new QLabel(this);
    reqMat->setGeometry(QRect(QPoint(0,0),
     QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    reqMat->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    reqMat->setFont(*baloo);											// Attribution de la police "baloo" au label
     reqMat->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 0px"); 				 // Attribution d'éléments de style CSS au texte
    reqMat->setText("Requêtes matérialisées");									// Attribution d'un texte au label
     reqMat->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

     reqMat->setFrameShape(QFrame::HLine);
     reqMat->setFrameStyle(QFrame::NoFrame);
    
    memoireReelleVal = new QLabel(this);
    memoireReelleVal->setGeometry(QRect(QPoint(0,0),
     QSize(120 * scaleWidthRatio, 30 * scaleHeigthRatio)));
    memoireReelleVal->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    
     memoireReelleVal->setFont(*baloo);										// Attribution de la police "baloo" au label
     memoireReelleVal->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 0px");     		 // Attribution d'éléments de style CSS au texte
    memoireReelleVal->setText(QString::fromStdString(to_string(espaceMemoireReel)));				// Attribution d'un texte au label
     memoireReelleVal->setAlignment(Qt::AlignCenter | Qt::AlignCenter);

     memoireReelleVal->setFrameShape(QFrame::HLine);
     memoireReelleVal->setFrameStyle(QFrame::NoFrame);
    
    reqMatVal = new QLabel(this);
    reqMatVal->setGeometry(QRect(QPoint(0,0),
     QSize(120 * scaleWidthRatio, 30 * scaleHeigthRatio)));
    reqMatVal->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    reqMatVal->setFont(*baloo);											// Attribution de la police "baloo" au label
     reqMatVal->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 0px"); 			 // Attribution d'éléments de style CSS au texte
    reqMatVal->setText(QString::fromStdString(to_string(0)));							// Attribution d'un texte au label
     reqMatVal->setAlignment(Qt::AlignCenter | Qt::AlignCenter);

     reqMatVal->setFrameShape(QFrame::HLine);
     reqMatVal->setFrameStyle(QFrame::NoFrame);
    
    
    QLabel* uniteMemoire = new QLabel(this);
    uniteMemoire->setGeometry(QRect(QPoint(0,0),
     QSize(200 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    uniteMemoire->setFrameStyle(QFrame::Panel | QFrame::Sunken);

     uniteMemoire->setFont(*baloo);										// Attribution de la police "baloo" au label
     uniteMemoire->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 18px"); 			 // Attribution d'éléments de style CSS au texte
     uniteMemoire->setText("Unités de mémoire prévues");							// Attribution d'un texte au label

     uniteMemoire->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
     uniteMemoire->setFrameShape(QFrame::HLine);
     uniteMemoire->setFrameStyle(QFrame::NoFrame);
    
    textMemoire = new QLabel(this);
    textMemoire->setGeometry(QRect(QPoint(0,0),
     QSize(120 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    textMemoire->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    textMemoire->setFont(*baloo);										// Attribution de la police "baloo" au label
     textMemoire->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 0px"); 			// Attribution d'éléments de style CSS au texte		
    textMemoire->setText(QString::fromStdString(to_string(0)));							// Attribution d'un texte au label
     textMemoire->setAlignment(Qt::AlignCenter | Qt::AlignCenter);

     textMemoire->setFrameShape(QFrame::HLine);
     textMemoire->setFrameStyle(QFrame::NoFrame);
    
     nbRequetesAMaterialiserBox =  new QLineEdit(this);
    nbRequetesAMaterialiserBox->setGeometry(QRect(QPoint(0,0),
     QSize(120 * scaleWidthRatio, 50 * scaleHeigthRatio)));

    nbRequetesAMaterialiserBox->setFont(*baloo);								// Attribution de la police "baloo" au label
     nbRequetesAMaterialiserBox->setStyleSheet("font-weight:medium; font-size:13pt; padding-left: 10px");	// Attribution d'éléments de style CSS au texte	
    nbRequetesAMaterialiserBox->setText(QString::fromStdString(to_string(0)));					// Attribution d'un texte au label
     nbRequetesAMaterialiserBox->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    
    nbRequetesAMaterialiserBox->setMinimumHeight(15 * scaleHeigthRatio);
    nbRequetesAMaterialiserBox->setMaximumHeight(15 * scaleHeigthRatio);
    nbRequetesAMaterialiserBox->setMinimumWidth(80 * scaleWidthRatio);
    nbRequetesAMaterialiserBox->setMaximumWidth(80 * scaleWidthRatio);
    
    nbRequetesAMaterialiserButton = new QPushButton(this);

    nbRequetesAMaterialiserButton->setFont(*baloo);								// Attribution de la police "baloo" au label
    nbRequetesAMaterialiserButton->setStyleSheet("QPushButton{background-color:#191970; border-radius:10px;font-weight:medium; font-size:13pt;color:white}QPushButton:hover{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:13pt;color:white} QPushButton:pressed{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:10pt;color:white}");// Attribution d'éléments de style CSS au texte	
    nbRequetesAMaterialiserButton->setText("OK");								// Attribution d'un texte au label
    nbRequetesAMaterialiserButton->setMaximumSize(70 * scaleWidthRatio, 30 * scaleHeigthRatio);
    nbRequetesAMaterialiserButton->setMinimumSize(70 * scaleWidthRatio, 30 * scaleHeigthRatio);
    connect(nbRequetesAMaterialiserButton, SIGNAL (released()), this, SLOT (calculRequetesAMateriliser()));
    
    nbMaterialiserLayout = new QGridLayout();
    nbMaterialiserLayout->setAlignment(Qt::AlignTop| Qt::AlignCenter);
    nbMaterialiserLayout->setVerticalSpacing(0);
    nbMaterialiserLayout->setRowMinimumHeight(0,45 * scaleHeigthRatio);
    nbMaterialiserLayout->addWidget(chiffre3,0,0,1,0, Qt::AlignTop| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserLabel,1,0, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(memoireReelleVal, 4, 1);
    nbMaterialiserLayout->addWidget(memoireReelle, 4, 0);
    nbMaterialiserLayout->addWidget(reqMat, 3, 0);
    nbMaterialiserLayout->addWidget(reqMatVal, 3, 1);
    nbMaterialiserLayout->addWidget(uniteMemoire,2,0, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(textMemoire,2,1, Qt::AlignCenter| Qt::AlignCenter);
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserButton,5,1, Qt::AlignCenter| Qt::AlignCenter);
}


void MainWindow::initExporterLayout() {
    
    exporterBack = new QLabel(this);
    exporterBack->setGeometry(QRect(QPoint(100, 30),
     QSize(300 * scaleWidthRatio, 300 * scaleHeigthRatio)));
    exporterBack->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    exporterBack->setText("");
     QPixmap pixmaRequete("../ui_resources/conteneur_blanc.png");
     exporterBack->setPixmap(pixmaRequete.scaled(exporterBack->width(),exporterBack->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     exporterBack->setAlignment(Qt::AlignCenter| Qt::AlignCenter);
     exporterBack->setFrameShape(QFrame::HLine);
     exporterBack->setFrameStyle(QFrame::NoFrame);
    

    QLabel* chiffre3 = new QLabel(this);
    chiffre3->setGeometry(QRect(QPoint(0,0),
     QSize(150 * scaleWidthRatio, 50 * scaleHeigthRatio)));
    chiffre3->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     QPixmap pixmapChiffre2("../ui_resources/exportcsv.png");
     chiffre3->setPixmap(pixmapChiffre2.scaled(chiffre3->width(),chiffre3->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     chiffre3->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     chiffre3->setFrameShape(QFrame::HLine);
     chiffre3->setFrameStyle(QFrame::NoFrame);  

    QLabel* selectionFichier = new QLabel(this);
    selectionFichier->setGeometry(QRect(QPoint(0,0),
     QSize(250 * scaleWidthRatio, 80 * scaleHeigthRatio)));
    selectionFichier->setFrameStyle(QFrame::Panel | QFrame::Sunken);

     selectionFichier->setFont(*baloo);										// Attribution de la police "baloo" au label
     selectionFichier->setStyleSheet("font-weight:medium; font-size:13pt ; padding-left: 8px"); 		// Attribution d'éléments de style CSS au texte	
     selectionFichier->setText("Souhaitez-vous exporter la table" "\n"  "de fait générée ?");			// Attribution d'un texte au label avec un saut de ligne "\n"
     selectionFichier->setAlignment(Qt::AlignTop | Qt::AlignLeft);
     selectionFichier->setFrameShape(QFrame::HLine);
     selectionFichier->setFrameStyle(QFrame::NoFrame);
    
    exporterButton = new QPushButton(this);

    exporterButton->setFont(*baloo);										// Attribution de la police "baloo" au label
    exporterButton->setStyleSheet("QPushButton{background-color:#191970; border-radius:10px;font-weight:medium; font-size:13pt;color:white}QPushButton:hover{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:13pt;color:white} QPushButton:pressed{background-color:#5b5b81; border-radius:10px;font-weight:medium; font-size:10pt;color:white}"); // Attribution d'éléments de style CSS au texte	
    exporterButton->setText("Exporter");										// Attribution d'un texte au label
    exporterButton->setMaximumSize(150 * scaleWidthRatio, 40 * scaleHeigthRatio);
    exporterButton->setMinimumSize(150 * scaleWidthRatio, 40 * scaleHeigthRatio);
    connect(exporterButton, SIGNAL (released()), this, SLOT (exporterButtonLaunch()));
    
    exporterLayout = new QGridLayout();
    exporterLayout->addWidget(chiffre3,0,0, Qt::AlignTop| Qt::AlignCenter);
    exporterLayout->addWidget(selectionFichier,1,0,Qt::AlignCenter| Qt::AlignCenter);
    exporterLayout->addWidget(exporterButton,2,0,Qt::AlignCenter| Qt::AlignCenter);
    
}


void MainWindow::request() {
    //Concurrent thread pour requeter les requêtes et pas bloquer le thread principal de la fenêtre graphique
    if(tableFaitString.size() == 0) {
        msgBox->setText("Commencez par importer une table avant de requêter");
        msgBox->show();
        return;
        
    }
    barRequest->setValue(0);
    barRequest->show();
    mainLayout->addWidget(barRequest,0,3, Qt::AlignTop | Qt::AlignRight);
    connect(this, SIGNAL(endRequest(long)), this, SLOT(requestBarUpdate(long)), Qt::BlockingQueuedConnection);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::doRequest );
}


vector<string> MainWindow::split(string &s, char delim) {
  s.erase(std::remove(s.begin(), s.end(), '\n'),s.end());
  stringstream ss(s);
  string item;
  vector<string> elems;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

void MainWindow::onClickChampsComboBox(int) {
    //Gestion de la selection de dimensions pour la requête
    string selected = champsRequetesComboBox->itemText(champsRequetesComboBox->currentIndex()).toUtf8().constData();
    champsRequetesComboBox->setCurrentIndex(0);
    if(!selected.empty()) {
        listeChampsRetenu->setText(listeChampsRetenu->toPlainText().toUtf8().constData() + QString::fromStdString(selected + ";") + "\n");
        QTextCursor cursor = listeChampsRetenu->textCursor();
        cursor.setPosition(listeChampsRetenu->toPlainText().size());
        listeChampsRetenu->setTextCursor(cursor);
        requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
    }
}

void MainWindow::effacerListeChamps() {
    listeChampsRetenu->setText("");
    requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
}

void MainWindow::tailleMaxVector(QString) {
    //Calcul des tailles des requêtes
    try {
        string value = nbRequetesAMaterialiserBox->text().toUtf8().constData();
        if(value.find(',') != std::string::npos) {
            textMemoire->setText(QString::fromStdString(to_string(0)));
            return;
        }
        if(value.find('.') != std::string::npos) {
            textMemoire->setText(QString::fromStdString(to_string(0)));
            return;
        }
        long selected = stol(value);
        if (selected > taillesRequetes.size()) {
            selected = taillesRequetes.size() - 1;
        }
        espaceMemoire = espaceMemoirePrevu(taillesRequetes, selected);
        textMemoire->setGeometry(QRect(QPoint(0,0),
         QSize(100 * scaleWidthRatio, 50 * scaleHeigthRatio)));
        textMemoire->setText(QString::fromStdString(to_string(espaceMemoire)));
        nbMaterialiserLayout->addWidget(textMemoire,2,1, Qt::AlignCenter| Qt::AlignCenter);
    } catch (exception& e) {
        textMemoire->setText(QString::fromStdString(to_string(0)));
        return;
    }
}

void MainWindow::runCaculRequete() {
    //Thread concurrent de calcul deds tailles
    endCalculReq = false;
    tempsMaterialisation = 0.0;
    clock_t begin = clock();
    try {
        string value = nbRequetesAMaterialiserBox->text().toUtf8().constData();
        if(value.find(',') != std::string::npos) {
            endCalculReq = true;
             emit endCalculRequete(-1);
            return;
        }
        if(value.find('.') != std::string::npos) {
            endCalculReq = true;
            emit endCalculRequete(-1);
            return;
        }
        nbAMateriliser = stol(value);
    } catch (exception& e) {
        endCalculReq = true;
        emit endCalculRequete(-1);
        return;
    }
    if (nbAMateriliser >= taillesRequetes.size() || nbAMateriliser < 0) {
        emit endCalculRequete(-1);
        return;
    }
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::processMaterialisation );
    requetesMaterialise = calculBeneficeTotal(taillesRequetes, nbAMateriliser);
    espaceMemoireReel = espaceMemoireUtilise(taillesRequetes, requetesMaterialise);
    emit endCalculRequete(45);
    stockerRequete(requetesMaterialise, tableFaitString, map_Sum, map_Max);
    endCalculReq = true;
    clock_t end = clock();
    tempsMaterialisation += (double)(end - begin)/CLOCKS_PER_SEC;
    emit endCalculRequete(100);
}


void MainWindow::displayPopupEndCalculRequete(long value){
    //Reception des messages des thread concurrent pour la mise à jour de l'ui
    if (value == -1) {
        string message = "Entrer un entier entre 0 - " + to_string(taillesRequetes.size()-1) + " !";
        msgBox->setText(QString::fromStdString(message));
        msgBox->show();
        barMat->setValue(100);
    }
    else if(value == 100) {
        string message = "Les requêtes sont préchargées ! <br><br>";
        if(nbAMateriliser > requetesMaterialise.size()){
            message +=  to_string(requetesMaterialise.size()-1) + " matérialisations suffisent à optimiser les requêtes utilisant " + to_string(espaceMemoireReel) + " d'unités de mémoire";
        }
        msgBox->setText(QString::fromStdString(message));
        msgBox->show();
        barMat->setValue(value);
        memoireReelleVal->setText(QString::fromStdString(to_string(espaceMemoireReel)));
        reqMatVal->setText(QString::fromStdString(to_string(requetesMaterialise.size()-1)));
        statusBar->clearMessage();
        statusBar->showMessage("Matérialisation en " + QString::fromStdString(to_string(tempsMaterialisation)) + "s" );
    }
    else{
        statusBar->clearMessage();
        statusBar->showMessage("Chargement... ");
        barMat->setValue(value);
    }
}

void MainWindow::runChargementFichier() {
    //Thread concurrent pour le chargement du fichier
    endChargeFichier = false;
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::processChargementFichier );
    tempsChargement = 0.0;
    clock_t begin = clock();
    if(barMat->value() > 0 && barMat->value() < 100) {
        endChargeFichier = true;
        emit endChargementFichier(-1);
        return;
    }
    string filePath = dirPath.toUtf8().constData();
    tableFaitString = chargerFichiers(filePath);
    if(tableFaitString.size() == 0) {
        endChargeFichier = true;
        emit endChargementFichier(-1);
        return;
    }
    emit endChargementFichier(10);
    tableFait = conversion(tableFaitString);
    if(tableFait.size() == 0) {
        endChargeFichier = true;
        emit endChargementFichier(-1);
        return;
    }
    emit endChargementFichier(50);
    taillesRequetes = toutes_les_tailles(tableFait);
    if(taillesRequetes.size() == 0) {
        endChargeFichier = true;
        emit endChargementFichier(-1);
        return;
    }
    endChargeFichier = true;
    clock_t end = clock();
    tempsChargement += (double)(end - begin)/CLOCKS_PER_SEC;
    emit endChargementFichier(100);
}

void MainWindow::displayPopupEndChargementFichier(long value) {
    //Reception des mise à jour de la barre de charhgement pour l'ui
    if(value == -1) {
        msgBox->setText(QString::fromStdString("Votre fichier n'est pas pris en charge, veuillez sélectionner un autre fichier !"));
        msgBox->show();
        bar->setValue(100);
        return;
    }
    if(value == 100) {
        initTableFaitView();
        initTableTailleRequetesWidget();
        tailleTableFait->setText(QString::fromStdString(to_string(tableFait.size())) + " lignes");
        msgBox->setText(QString::fromStdString("Ouverture et chargement du fichier : ") + dirPath.toUtf8().constData() + QString::fromStdString("  <br> <br> Votre fichier a été importé avec succès !"));
        msgBox->show();
        statusBar->clearMessage();
        statusBar->showMessage("Chargement du fichier en " + QString::fromStdString(to_string(tempsChargement)) + "s" );
        bar->setValue(value);
        return;
    }
    statusBar->clearMessage();
    statusBar->showMessage("Chargement ... ");
    bar->setValue(value);
}

void MainWindow::exporterButtonLaunch() {
    //Lancement de l'exportation
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
    barExport->setValue(0);
    barExport->show();
    connect(this, SIGNAL(endExportFichier(long)), this, SLOT(displayPopupExporter(long)), Qt::BlockingQueuedConnection);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::exporter );
}

void MainWindow::exporter() {
    //Thread concurrent de l'export de fichier
    emit endExportFichier(0);
    string path = saveDirPath.toUtf8().constData();
    exportFichier(tableFaitRequete, path);
    emit endExportFichier(100);
}

void MainWindow::displayPopupExporter(long value) {
    //Mise à jour des infos de la barre de progressipn sur l'ui
    barExport->setValue(value);
    if(value == 100) {
        msgBox->setText(QString::fromStdString("Fichier enregistré !"));
        msgBox->show();
    }
}

void MainWindow::doRequest() {
    //Thread concurrent de traitement d'une requete
    tempsRequete = 0.0;
    clock_t begin = clock();
    
    string request = listeChampsRetenu->toPlainText().toUtf8().constData();
    vector<string> requete = split(request,';');
    long operation = 0;
    string fonctionAggregation = fonctionAggregationComboBox->itemText(fonctionAggregationComboBox->currentIndex()).toUtf8().constData();
    emit endRequest(20);
    if( fonctionAggregation == "Max")
        operation = 1;
    emit endRequest(25);
    materialiserRequete(requete,taillesRequetes, requetesMaterialise, tableFaitString, map_Sum, map_Max, operation, tableFaitRequete);
    clock_t end = clock();
    tempsRequete += (double)(end - begin)/CLOCKS_PER_SEC;
    emit endRequest(100);
}


void MainWindow::requestBarUpdate(long value) {
    //Mise à jour de la barre de progression sur l'ui
    barRequest->setValue(value);
    if(value == 100) {
        listeChampsRetenu->setText(listeChampsRetenu->toPlainText().toUtf8().constData());
        requeteLayout->addWidget(listeChampsRetenu,3,0,2,1,Qt::AlignCenter| Qt::AlignRight);
        tailleResultatRequete->setText(QString::fromStdString(to_string(tableFaitRequete.size()-1)) + " lignes");
        mainLayout->addWidget(tailleResultatRequete, 5, 2);
        if(this->tableFaitRequete.size() > 1000) {
            tableFaitRequeteWidget->setRowCount(1000);
        }
        else tableFaitRequeteWidget->setRowCount(this->tableFaitRequete.size()-1);
        tableFaitRequeteWidget->setColumnCount(this->tableFaitRequete[0].size());
        for(long i = 0; i<tableFaitRequete[0].size(); i++) {
           tableFaitRequeteWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromStdString(tableFaitRequete[0][i])));
        }
        for(long i = 1; i<tableFaitRequete.size(); i++) {
            for(long j = 0; j<tableFaitRequete[0].size(); j++){
                QTableWidgetItem *item = tableFaitRequeteWidget->item(-1, j);
                if(!item) {
                    item = new QTableWidgetItem();
                    tableFaitRequeteWidget->setItem(i-1, j, item);
                }
                item->setText(QString::fromStdString(tableFaitRequete[i][j]));
            }
        }
        tableFaitRequeteWidget->resizeColumnsToContents();
        tableFaitRequeteWidget->resizeRowsToContents();
        
        string optimisation = "Requête optimisée : ";
        if (requetesMaterialise.size() <= 1) {
            optimisation = "Requête non optimisée : ";
        }
        tempsReq->setText(QString::fromStdString(optimisation) + QString::fromStdString(to_string(tempsRequete)) + "s");
        mainLayout->addWidget(tempsReq, 2, 3);
    }
}

void MainWindow::info() {
    //Info de l'application html
    QMessageBox msgInfo(this);
    msgInfo.setWindowTitle("A Propos de");
    msgInfo.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgInfo.setText("<h1 style='text-align=center'>Bienvenue sur QueryOptimizer</h1> <p>Version 2.0.0 <br> <br> <a href='https://github.com/EzekielaRakotoarijaona/OlapMod/issues'>Support</a><br><br><img src='../ui_resources/univ.png' width='150' heigth='100' ><br>&copy; 2020 <br> <br> Créé par : Oubeyy Shah, Lilia Naitamara, Riad M'himdi, Claire-Elise Hochet, Camille Rakotoarijaona <br> <br> <a href='https://github.com/EzekielaRakotoarijaona/OlapMod/blob/master/LICENSE'>Licence Apache-2.0 <br><br> </a>");
    msgInfo.exec();
}


void MainWindow::help() {
    //Aide de l'application au format html
    QWidget *helpWindow = new QWidget;
    QGridLayout* layout = new QGridLayout;
    QTextBrowser *tb = new QTextBrowser(this);
    tb->setOpenExternalLinks(true);
    QFile file("../doc/aide.html");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      qWarning("file not found");
      return;
    }
    tb->setHtml(file.readAll());
    helpWindow->setLayout(layout);
    layout->addWidget(tb);
    helpWindow->show();
}

void MainWindow::processMaterialisation() {
    while(!endCalculReq) {
        if(nbAMateriliser != 0) {
            int value = 50 + ((map_Sum.size() + map_Max.size()) * 50 / (nbAMateriliser * 2));
            if(value != 100) emit endCalculRequete(value);
        }
        if (map_Sum.size() == nbAMateriliser && map_Max.size() == nbAMateriliser) {
            return;
        }
    }
}

void MainWindow::processChargementFichier() {
    while(!endChargeFichier) {
        if( tableFait.size() != 0) {
            int value =  tableFait.size() * 50 / tableFaitString.size();
            if (value != 100) emit endChargementFichier(value);
        }
        if(tableFait.size() == tableFaitString.size()) return;
    }
}




