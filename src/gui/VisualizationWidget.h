#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <memory>
#include <iostream>

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "Volume.h"

class VisualizationWidget : public QWidget
{
    Q_OBJECT
private:
    QGridLayout _mainLayout;
    //loads the Volume from a file
    QPushButton _loadFileButton;
    
    //loads the Volume from the ReconstructionPose
    QPushButton _loadRecButton;
    
    void loadFromFile(){
        
    }
    
public:
    
    VisualizationWidget() : _mainLayout{},_loadFileButton {"Load from file"}, _loadRecButton {"Load acquisition"}{
        _mainLayout.addWidget(&_loadRecButton);
        setLayout(&_mainLayout);
        connect(&_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);
    }
    
    void setRec(const std::shared_ptr<const Volume>& vol){
        //TODO do some real stuff here
        std::cout << "Did it! " << vol->getNumVoxels() << std::endl; 
    }
    
signals:
    //emit this event if you want the volume from the requisition. It is set through a call of setRec
    void requestRecVolume();
    
    
};

#endif // VISUALIZATIONWIDGET_H
