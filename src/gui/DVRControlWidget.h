#ifndef DVRCONTROLWIDGET_H
#define DVRCONTROLWIDGET_H


#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QColorDialog>
#include <QObject>
#include <QGroupBox>

#include "DVRModel.h"
#include "BoxSlider.h"
#include "TransferFuncWidget.h"

class DVRControlWidget : public QGroupBox
{
    Q_OBJECT
private:
    DVRModel& _model;
    CameraPose& _camera;
    
    BoxSlider* _step;
    BoxSlider* _rotY;
    BoxSlider* _rotZ;
    BoxSlider* _zoom;
    BoxSlider* _distance;
    
    QCheckBox* _perspective;
    
    TransferFuncWidget* _func;

    QGridLayout* _layout;
    
    
public:
    DVRControlWidget(DVRModel& model);

private slots:
    void updateRotationY();
    void updateRotationZ();
    void updateZoom();
    void updateDistance();
    void updateStepSize();

signals:
    void rotationYChanged(float y);
    void rotationZChanged(float z);
    void zoomChanged(float zoom);
    void distanceChanged(float distance);
    
};

#endif