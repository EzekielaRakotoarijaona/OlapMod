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


using namespace std;

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
    initiChargerLayout();
    initRequeteLayout();
    nbMterialisationLayout();
    initExporterLayout();
    
   title = new QLabel(this);
   title->setGeometry(QRect(QPoint(100, 30),
    QSize(200, 50)));
   title->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   title->setText("");
    QPixmap pixmap("../ui_resources/QueryOptimizer.png");
    title->setPixmap(pixmap.scaled(title->width(),title->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    title->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    title->setFrameShape(QFrame::HLine);
    title->setFrameStyle(QFrame::NoFrame);
    
    titleTableFait = new QLabel(this);
    titleTableFait->setGeometry(QRect(QPoint(100, 30),
     QSize(200, 50)));
    titleTableFait->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    titleTableFait->setText("");
     QPixmap pixmapTitleTableFait("../ui_resources/table_fait.png");
     titleTableFait->setPixmap(pixmapTitleTableFait.scaled(titleTableFait->width(),titleTableFait->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     titleTableFait->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     titleTableFait->setFrameShape(QFrame::HLine);
     titleTableFait->setFrameStyle(QFrame::NoFrame);
    
    titleTableGeneree = new QLabel(this);
    titleTableGeneree->setGeometry(QRect(QPoint(100, 30),
     QSize(200, 50)));
    titleTableGeneree->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    titleTableGeneree->setText("");
     QPixmap pixmapTitleTableGeneree("../ui_resources/table_fait_generee.png");
     titleTableGeneree->setPixmap(pixmapTitleTableGeneree.scaled(titleTableGeneree->width(),titleTableGeneree->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     titleTableGeneree->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
     titleTableGeneree->setFrameShape(QFrame::HLine);
     titleTableGeneree->setFrameStyle(QFrame::NoFrame);
    
    
    mainLayout = new QGridLayout();
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->setRowMinimumHeight(4, 400);
    mainLayout->addWidget(title);
    mainLayout->addWidget(chargerBack,1,0);
    mainLayout->addLayout(chargerLayout, 1,0);
    mainLayout->addWidget(requeteBack,1,1);
    mainLayout->addLayout(requeteLayout, 1,1);
    mainLayout->addWidget(nbMaterialiserBack, 1,2);
    mainLayout->addLayout(nbMaterialiserLayout, 1,2);
    mainLayout->addWidget(exporterBack, 1,3);
    mainLayout->addLayout(exporterLayout, 1,3);
    mainLayout->addWidget(titleTableFait, 2,0,2,1);
    mainLayout->addWidget(titleTableGeneree, 2,2,2,1);

    QWidget* contentWidget = new QWidget();
    contentWidget->setLayout(mainLayout);

    mainScrollArea = new QScrollArea();
    mainScrollArea->setWidget(contentWidget);
    mainScrollArea->resize(2000,4000);

    setCentralWidget(mainScrollArea);
}

void MainWindow::handleButton()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Charger une table de fait ..."),tr("BDD (*.csv)"));
    QMessageBox msgBox;
    if(file.isEmpty()) {
        msgBox.setText("Le fichier est vide ou n'existe pas");
        return;
    }
     
    QFileInfo fileInfo(file);
    QString dirPath = fileInfo.filePath(); // Path vers le fichier
    QString fileName = fileInfo.fileName();
    string filePath = dirPath.toUtf8().constData();
    this->tableFaitString = chargerFichiers(filePath);
    tableFait = conversion(tableFaitString);
    initTableFaitView();
    initTableTailleRequetesWidget();
    msgBox.setText(QString::fromStdString("Ouverture et chargement du fichier :  ") + dirPath.toUtf8().constData() + QString::fromStdString(" termninée !"));
    msgBox.exec();
}

void MainWindow::initTableFaitView(){
    tableFaitWidget = new QTableWidget();
    tableFaitWidget->setRowCount(this->tableFaitString.size()-1);
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
    tableFaitWidget->setMaximumWidth(600);
    tableFaitWidget->setMinimumWidth(600);
    tableFaitWidget->setMaximumHeight(1500);
    tableFaitWidget->setMinimumHeight(1500);
    tableFaitWidget->resizeColumnsToContents();
    tableFaitWidget->resizeRowsToContents();
    
    mainLayout->addWidget(tableFaitWidget, 4,0,1,2, Qt::AlignTop| Qt::AlignLeft);
    
    delete champsRequetesComboBox;
    champsRequetesComboBox =  new QComboBox(this);
    for(int i = 0; i< tableFaitString[0].size() - 1; i++) {
        champsRequetesComboBox->addItem(QString::fromStdString(tableFaitString[0][i]));
    }
    champsRequetesComboBox->setGeometry(QRect(QPoint(650, 100),
    QSize(100, 50)));
    requeteLayout->addWidget(champsRequetesComboBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
}

void MainWindow::initTableTailleRequetesWidget(){
    taillesRequetes = toutes_les_tailles(tableFait);
    requetesMaterialise.resize(1);
    requetesMaterialise[0] = taillesRequetes.size()-1;
    delete nbRequetesAMaterialiserBox;
    nbRequetesAMaterialiserBox =  new QComboBox(this);
    for(int i = 0; i< taillesRequetes.size(); i++) {
        nbRequetesAMaterialiserBox->addItem(QString::fromStdString(to_string(i)));
    }
    nbRequetesAMaterialiserBox->setGeometry(QRect(QPoint(650, 100),
    QSize(100, 50)));
    nbMaterialiserLayout->addWidget(nbRequetesAMaterialiserBox,1,1, Qt::AlignCenter| Qt::AlignCenter);
}


void MainWindow::calculRequetesAMateriliser() {
    int nbAMateriliser = stoi(nbRequetesAMaterialiserBox->itemText(nbRequetesAMaterialiserBox->currentIndex()).toUtf8().constData());
    requetesMaterialise = calculBeneficeTotal(taillesRequetes, nbAMateriliser);
    stockerRequete(requetesMaterialise, tableFaitString, map_Sum, map_Max);
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString("Les requetes sont préchargées !"));
    msgBox.exec();
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
     QPixmap pixmapSelectionFichier("../ui_resources/selection_fichier.png");
     selectionFichier->setPixmap(pixmapSelectionFichier.scaled(selectionFichier->width(),selectionFichier->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     selectionFichier->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     selectionFichier->setFrameShape(QFrame::HLine);
     selectionFichier->setFrameStyle(QFrame::NoFrame);
    
    m_button = new QPushButton("", this);
    m_button->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_charger_fichier.png); } ");
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
      QPixmap pixmapChamps("../ui_resources/les_champs.png");
      champsRequetes->setPixmap(pixmapChamps.scaled(champsRequetes->width(),champsRequetes->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
      champsRequetes->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      champsRequetes->setFrameShape(QFrame::HLine);
      champsRequetes->setFrameStyle(QFrame::NoFrame);
     
     QLabel* fonctionAggregation = new QLabel(this);
     fonctionAggregation->setGeometry(QRect(QPoint(0,0),
      QSize(100, 50)));
     fonctionAggregation->setFrameStyle(QFrame::Panel | QFrame::Sunken);
      QPixmap pixmapAggregation("../ui_resources/operation.png");
      fonctionAggregation->setPixmap(pixmapAggregation.scaled(fonctionAggregation->width(),fonctionAggregation->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
    validerRequete->setMaximumSize(50, 30);
    validerRequete->setMinimumSize(50, 30);
    connect(validerRequete, SIGNAL (released()), this, SLOT (request()));
    
    effacerChamps = new QPushButton("", this);
    effacerChamps->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_effacer.png); } ");
    effacerChamps->setMaximumSize(50, 30);
    effacerChamps->setMinimumSize(50, 30);
    connect(effacerChamps, SIGNAL (released()), this, SLOT (handleButton()));
    
     
     
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
    nbMaterialiser->setGeometry(QRect(QPoint(0,0),
     QSize(120, 50)));
    nbMaterialiser->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     QPixmap pixmapNbMaterialiser("../ui_resources/nb_materialiser.png");
     nbMaterialiser->setPixmap(pixmapNbMaterialiser.scaled(nbMaterialiser->width(),nbMaterialiser->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     nbMaterialiser->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     nbMaterialiser->setFrameShape(QFrame::HLine);
     nbMaterialiser->setFrameStyle(QFrame::NoFrame);
     nbRequetesAMaterialiserBox =  new QComboBox(this);
    
    QLabel* uniteMemoire = new QLabel(this);
    uniteMemoire->setGeometry(QRect(QPoint(0,0),
     QSize(100, 50)));
    uniteMemoire->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     QPixmap pixmapMemoire("../ui_resources/unite_memoire.png");
     uniteMemoire->setPixmap(pixmapMemoire.scaled(uniteMemoire->width(),uniteMemoire->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     uniteMemoire->setAlignment(Qt::AlignTop | Qt::AlignCenter);
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
    nbRequetesAMaterialiserButton->setMaximumSize(50, 30);
    nbRequetesAMaterialiserButton->setMinimumSize(50, 30);
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
     QPixmap pixmapSelectionFichier("../ui_resources/exporter.png");
     selectionFichier->setPixmap(pixmapSelectionFichier.scaled(selectionFichier->width(),selectionFichier->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
     selectionFichier->setAlignment(Qt::AlignTop | Qt::AlignCenter);
     selectionFichier->setFrameShape(QFrame::HLine);
     selectionFichier->setFrameStyle(QFrame::NoFrame);
    
    exporterButton = new QPushButton(this);
    exporterButton->setStyleSheet("QPushButton {border-image: url(../ui_resources/bouton_exporter.png); } ");
    exporterButton->setMaximumSize(100, 40);
    exporterButton->setMinimumSize(100, 40);
    connect(exporterButton, SIGNAL (released()), this, SLOT (handleButton()));
    
    exporterLayout = new QGridLayout();
    exporterLayout->addWidget(chiffre3,0,0, Qt::AlignTop| Qt::AlignCenter);
    exporterLayout->addWidget(selectionFichier,1,0,Qt::AlignCenter| Qt::AlignCenter);
    exporterLayout->addWidget(exporterButton,2,0,Qt::AlignCenter| Qt::AlignCenter);
    
}


void MainWindow::request() {
    string request = listeChampsRetenu->toPlainText().toUtf8().constData();
    vector<int> binaireRequete = {0,1,1};
    materialiserRequete(binaireRequete,taillesRequetes, requetesMaterialise, tableFaitString, map_Sum, map_Max, 0, tableFaitRequete);
    tableFaitRequeteWidget = new QTableWidget();
    tableFaitRequeteWidget->setRowCount(this->tableFaitRequete.size()-1);
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
    tableFaitRequeteWidget->setMaximumWidth(600);
    tableFaitRequeteWidget->setMinimumWidth(600);
    tableFaitRequeteWidget->setMaximumHeight(1500);
    tableFaitRequeteWidget->setMinimumHeight(1500);
    tableFaitRequeteWidget->resizeColumnsToContents();
    tableFaitRequeteWidget->resizeRowsToContents();
    
    mainLayout->addWidget(tableFaitRequeteWidget, 4,2,1,2, Qt::AlignTop| Qt::AlignLeft);
}
