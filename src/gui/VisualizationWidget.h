#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <memory>
#include <iostream>

#include <QColorDialog>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "DVRWidget.h"
#include "MPRWidget.h"
#include "VisualizationModel.h"
#include "Volume.h"

class VisualizationWidget : public QWidget
{
    Q_OBJECT
private:
    ///manages all data related to visualization
    VisualizationModel _visModel;

    ///all layout items for visulisation are composed in the main layout
    QGridLayout _mainLayout;
    ///manages all menu items
    QVBoxLayout _menuLayout;
    ///loads the Volume from a file
    QPushButton _loadFileButton;    
    ///loads the Volume from the ReconstructionPose
    QPushButton _loadRecButton;
    ///manages the color-choosing related widgets
    QHBoxLayout _colorLayout;
    ///displays the current color
    QLabel _colorLabel;
    ///opens a dialog to choose the color for visualization
    QPushButton _selectColorButton;

    ///renders the 2D MPR visualization
    MPRWidget _mprWidget;
    ///paints the direct volume rendering (DVR)
    DVRWidget _dvrWidget;
    
    void loadFromFile(){
        
    }

    void updateColorLabel()
    {
        _colorLabel.setStyleSheet( "background-color: " + _visModel.color().name() );
    }
    
public:
    
    VisualizationWidget() :
        _mainLayout {},
        _menuLayout {},
        _loadFileButton {"Load from file"},
        _loadRecButton {"Load acquisition"},
        _colorLayout {},
        _colorLabel {},
        _mprWidget {},
        _dvrWidget {},
        _selectColorButton {"Select color"}
    {
        updateColorLabel();
        _colorLayout.addWidget(&_colorLabel);
        _colorLayout.addWidget(&_selectColorButton);
        _menuLayout.addItem(&_colorLayout);

        _menuLayout.addWidget(&_loadFileButton);
        _menuLayout.addWidget(&_loadRecButton);

        _mainLayout.addItem(&_menuLayout, 0, 0);

        _mainLayout.addWidget(&_mprWidget, 0, 1);
        _mainLayout.addWidget(&_dvrWidget, 0, 2);
        setLayout(&_mainLayout);
        connect(&_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);
        connect(&_selectColorButton, &QPushButton::pressed, this, &VisualizationWidget::changeColor);
    }
    
    void setRec(const std::shared_ptr<const Volume>& vol){
        //TODO do some real stuff here
        std::cout << "Did it! " << vol->getNumVoxels() << std::endl; 
    }
    
signals:
    //emit this event if you want the volume from the requisition. It is set through a call of setRec
    void requestRecVolume();
    
public slots:
    void changeColor();
};

#endif // VISUALIZATIONWIDGET_H
