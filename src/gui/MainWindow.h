#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QImage>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "AcquisitionWidget.h"
#include "PoseViewer.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(std::string path)
        : _aWidget(path)
    {
        QTabWidget* tabWidget = new QTabWidget();

        tabWidget->addTab(&_aWidget, tr("Acquisition"));
        setCentralWidget(tabWidget);
    }


public slots:
    
private slots:
    
private:
    AcquisitionWidget _aWidget;

};

#endif // MAINWINDOW_H
