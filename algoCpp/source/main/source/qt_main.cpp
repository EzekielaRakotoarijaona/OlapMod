//
//  qt_main.cpp
//  
//
//  Created by Ezekiela Rakotoarijaona on 18/04/2020.
//

#include <stdio.h>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.resize(1200, 500);
    window.show();
    window.setWindowTitle(
        QApplication::translate("toplevel", "Top-level widget"));
    return app.exec();
}
