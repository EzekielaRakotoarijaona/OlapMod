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


using namespace std;

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
   // Create the button, make "this" the parent
   m_button = new QPushButton("Importer une table de fait", this);
   // set size and location of the button
   m_button->setGeometry(QRect(QPoint(100, 100),
   QSize(200, 50)));
    
   title = new QLabel(this);
   title->setGeometry(QRect(QPoint(500, 30),
    QSize(200, 50)));
   title->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   title->setText("Welcome to Query Optimizer !");
   title->setAlignment(Qt::AlignBottom | Qt::AlignRight);

   // Connect button signal to appropriate slot
   connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
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
    tableFaitWidget = new QTableWidget(this);
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
    QSize(300, 300)));
    tableFaitWidget->resizeColumnsToContents();
    tableFaitWidget->resizeRowsToContents();
    tableFaitWidget->show();
}

void MainWindow::initTableTailleRequetesWidget(){
    taillesRequetes = toutes_les_tailles(tableFait);
    
    tableTailleRequetesWidget = new QTableWidget(this);
    tableTailleRequetesWidget->setRowCount(this->taillesRequetes.size());
    tableTailleRequetesWidget->setColumnCount(2);
    tableTailleRequetesWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Requetes"));
    tableTailleRequetesWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Tailles"));
    for(int i = 0; i<tableFaitString.size(); i++) {
        QTableWidgetItem *item1 = tableTailleRequetesWidget->item(i, 0);
        QTableWidgetItem *item2 = tableTailleRequetesWidget->item(i, 1);
        if(!item1) {
            item1 = new QTableWidgetItem();
            tableTailleRequetesWidget->setItem(i, 0, item1);
        }
        item1->setText(QString::fromStdString(to_string(i)));
        if(!item2) {
            item2 = new QTableWidgetItem();
            tableTailleRequetesWidget->setItem(i, 1, item2);
        }
        item2->setText(QString::fromStdString(to_string(taillesRequetes[i])));
        
    }
    tableTailleRequetesWidget->setGeometry(QRect(QPoint(400, 200),
    QSize(300, 300)));
    tableTailleRequetesWidget->resizeColumnsToContents();
    tableTailleRequetesWidget->resizeRowsToContents();
    tableTailleRequetesWidget->show();
    
    
    nbRequetesAMaterialiserLabel = new QLabel(this);
    nbRequetesAMaterialiserLabel->setGeometry(QRect(QPoint(400, 100),
     QSize(250, 50)));
    nbRequetesAMaterialiserLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    nbRequetesAMaterialiserLabel->setText("Combien de requetes matérialiser ?");
    nbRequetesAMaterialiserLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    nbRequetesAMaterialiserLabel->show();
    
    nbRequetesAMaterialiserBox =  new QComboBox(this);
    for(int i = 0; i< taillesRequetes.size(); i++) {
        nbRequetesAMaterialiserBox->addItem(QString::fromStdString(to_string(i)));
    }
    nbRequetesAMaterialiserBox->setGeometry(QRect(QPoint(650, 100),
    QSize(100, 50)));
    nbRequetesAMaterialiserBox->show();
    
    
    nbRequetesAMaterialiserButton = new QPushButton("Calculer", this);
    nbRequetesAMaterialiserButton->setGeometry(QRect(QPoint(750, 100),
    QSize(200, 50)));
    nbRequetesAMaterialiserButton->show();
    connect(nbRequetesAMaterialiserButton, SIGNAL (released()), this, SLOT (calculRequetesAMateriliser()));
}


void MainWindow::calculRequetesAMateriliser() {
    int nbAMateriliser = stoi(nbRequetesAMaterialiserBox->itemText(nbRequetesAMaterialiserBox->currentIndex()).toUtf8().constData());
    requetesMaterialise = calculBeneficeTotal(taillesRequetes, nbAMateriliser);
    
    tableRequetesAMaterialiser = new QTableWidget(this);
    tableRequetesAMaterialiser->setRowCount(this->requetesMaterialise.size());
    tableRequetesAMaterialiser->setColumnCount(1);
    tableRequetesAMaterialiser->setHorizontalHeaderItem(0, new QTableWidgetItem("Requetes"));
    
    for(int i = 0; i<requetesMaterialise.size(); i++) {
        QTableWidgetItem *item1 = tableRequetesAMaterialiser->item(i, 0);
        if(!item1) {
            item1 = new QTableWidgetItem();
            tableRequetesAMaterialiser->setItem(i, 0, item1);
        }
        item1->setText(QString::fromStdString(to_string(requetesMaterialise[i])));
    }
    
    tableRequetesAMaterialiser->setGeometry(QRect(QPoint(750, 200),
    QSize(300, 300)));
    tableRequetesAMaterialiser->resizeColumnsToContents();
    tableRequetesAMaterialiser->resizeRowsToContents();
    tableRequetesAMaterialiser->show();
}
