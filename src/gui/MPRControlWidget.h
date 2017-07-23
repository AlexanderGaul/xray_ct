#ifndef MPRCONTROLWIDGET_H
#define MPRCONTROLWIDGET_H

#include <iostream>

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QGridLayout>
#include <QColorDialog>
#include <QObject>
#include <QGroupBox>

#include "MPRModel.h"
#include "BoxSlider.h"
#include "TransferFuncWidget.h"

/**
 * Contains a collection of widgets, that are used to manipulate the MPR.
 */

class MPRControlWidget : public QGroupBox
{
    Q_OBJECT
private:
    MPRModel& _model;
    
    
    BoxSlider* _posX;
    BoxSlider* _posY;
    BoxSlider* _posZ;
    
    BoxSlider* _rotY;
    BoxSlider* _rotZ;
    
    BoxSlider* _zoom;
    
    TransferFuncWidget* _func;


    QGridLayout* _layout;
    
    
public:
    MPRControlWidget(MPRModel& model);

    void setTransferFunctionRange(float from, float to);

public slots:
    void updatePositionX();
    void updatePositionY();
    void updatePositionZ();
    void updateRotationY();
    void updateRotationZ();
    
    void updateZoom();
    
    void changedVolume(const Volume& vol);

    void resetTransferFunction();

signals:
    void requestTransferFunctionReset();
};

#endif
