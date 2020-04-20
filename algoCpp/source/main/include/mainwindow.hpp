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
#include "../include/nbEnregistrements.hpp"
#include "../include/calculBenefice.hpp"


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
        void initTableTailleRequetesWidget();
    private slots:
       void handleButton();
       void calculRequetesAMateriliser();
    private:
        QLabel *title;
        QLabel *nbRequetesAMaterialiserLabel;
        QComboBox *nbRequetesAMaterialiserBox;
        QPushButton *nbRequetesAMaterialiserButton;
        QPushButton *m_button;
        QTableWidget *tableFaitWidget;
        QTableWidget *tableTailleRequetesWidget;
        QTableWidget *tableRequetesAMaterialiser;
        vector<vector<string>> tableFaitString;
        vector<int> taillesRequetes;
        vector<int> requetesMaterialise;
        matrice tableFait;
};

#endif /* mainwindow_hpp */
    
