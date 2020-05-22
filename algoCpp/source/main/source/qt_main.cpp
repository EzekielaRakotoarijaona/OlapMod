//
//  qt_main.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 18/04/2020.
//

#include <stdio.h>
#include "../include/mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.showMaximized();
    mainWindow.setWindowTitle("QueryOptimizer");
    return app.exec();
}
