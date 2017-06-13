#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QImage>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "AcquisitionWidget.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(std::string path)
        : _aWidget(path)
    {
        QVBoxLayout* layout = new QVBoxLayout;

        QHBoxLayout* topLayout = new QHBoxLayout;

        _aButton = new QPushButton("ACQ");
        _rButton = new QPushButton("REC");
        _vButton = new QPushButton("VIS");

        topLayout->addWidget(_aButton);
        topLayout->addWidget(_rButton);
        topLayout->addWidget(_vButton);

        layout->addItem(topLayout);

        QWidget* rootWidget = new QWidget;
        rootWidget->setLayout(layout);
        setCentralWidget(rootWidget);
    }


public slots:
    
private slots:
    
private:
    AcquisitionWidget _aWidget;
    QPushButton* _aButton;
    QPushButton* _rButton;
    QPushButton* _vButton;
};

#endif // MAINWINDOW_H
