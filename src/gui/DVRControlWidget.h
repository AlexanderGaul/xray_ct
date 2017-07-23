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
#include "Volume.h"

/**
 * Collection of widgets that are used to manipulate the DVR.
 */

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
    
    void resetTransferFunction()
    {
        emit requestTransferFunctionReset();
    }
    
public:
    DVRControlWidget(DVRModel& model);
    void setTransferFunctionRange(float from, float to);

private slots:
    // Used when the respective values in the model have changed and they have to be updated in the gui.
    void updateRotationY();
    void updateRotationZ();
    void updateZoom();
    void updateDistance();
    void updateStepSize();
    // Used when the volume has changed and slides might need to change their range
    void changedVolume(const Volume& vol);
signals:
/*
    // Emitted when the respective values have been set in the gui and they need to be set in the model
    void rotationYChanged(float y);
    void rotationZChanged(float z);
    void zoomChanged(float zoom);
    void distanceChanged(float distance);
*/
    
    void requestTransferFunctionReset();
};

#endif
