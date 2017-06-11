
#include <iostream>

#include <QApplication>

#include "gui/MainWindow.h"

int main(int argc, char *argv[]){
    if(argc < 2)
    {
        std::cerr << "ERROR: \t No .edf image file provided" << std::endl;
        return 1;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.loadImage(argv[1]);
    w.show();

    return a.exec();
}
