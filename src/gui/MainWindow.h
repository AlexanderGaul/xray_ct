#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "AcquisitionWidget.h"

class MainWindow : public QMainWindow {
public:
    MainWindow() : QMainWindow {} {
        
    }
public slots:
    
private slots:
    
private:
    AcquisitionWidget aWidget;
};

#endif // MAINWINDOW_H
