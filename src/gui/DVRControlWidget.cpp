#include "DVRControlWidget.h"

#include <iostream>

DVRControlWidget::DVRControlWidget(DVRModel& model)
    :
    QGroupBox {"Direct Volume Rendering"},
    _model {model},
    _camera {model.getCameraPose()},
    _rotY {new BoxSlider(0.f, 360.f, 1, 2.5f)},
    _rotZ {new BoxSlider(0.f, 360.f, 1, 2.5f)},
    _zoom {new BoxSlider(0.f, 200.f, 2, 2.f )},
    _distance {new BoxSlider(0.f, 10.f, 4, 0.005f)},
    _step {new BoxSlider(0.f, 1.f, 6, 0.0005f)},
    _perspective {new QCheckBox()},
    _layout {new QGridLayout()}
{
    
    QLabel* rotYLabel  = new QLabel("Y Rotation");
    QLabel* rotZLabel  = new QLabel("Z Rotation");
    QLabel* zoomLabel  = new QLabel("Zoom");
    QLabel* distanceLabel = new QLabel("Distance");
    QLabel* stepLabel = new QLabel("Step Size");
    QLabel* perspectiveLabel = new QLabel("Perspective");
    
    _layout->addWidget(perspectiveLabel, 0, 0);
    _layout->addWidget(rotYLabel, 1, 0);
    _layout->addWidget(rotZLabel, 2, 0);
    _layout->addWidget(zoomLabel, 3, 0);
    _layout->addWidget(distanceLabel, 4, 0);
    _layout->addWidget(stepLabel, 5, 0);
    
    _layout->addWidget(_perspective, 0, 1);
    _layout->addWidget(_rotY, 1, 1);
    _layout->addWidget(_rotZ, 2, 1);
    _layout->addWidget(_zoom, 3, 1);
    _layout->addWidget(_distance, 4, 1);
    _layout->addWidget(_step, 5, 1);
    
    _layout->setAlignment(Qt::AlignTop);
    
    
    
    setLayout(_layout);
    
    //connects
    connect(_perspective, &QCheckBox::stateChanged, &model, &DVRModel::setPerspective);
    connect(_rotY, &BoxSlider::valueChanged, &model, &DVRModel::setRotationYDeg);
    connect(_rotZ, &BoxSlider::valueChanged, &model, &DVRModel::setRotationZDeg);
    connect(_distance, &BoxSlider::valueChanged, &model, &DVRModel::setDistance);
    connect(_zoom, &BoxSlider::valueChanged, &model, &DVRModel::setZoom);
    connect(_step, &BoxSlider::valueChanged, &_model, &DVRModel::setStepSize);
    
    
    connect(&model, &DVRModel::updateRotationY, this, &DVRControlWidget::updateRotationY);
    connect(&model, &DVRModel::updateRotationZ, this, &DVRControlWidget::updateRotationZ);
    connect(&model, &DVRModel::updateZoom, this, &DVRControlWidget::updateZoom);
    connect(&model, &DVRModel::updateDistance, this ,&DVRControlWidget::updateDistance);
    connect(&model, &DVRModel::updateStepSize, this ,&DVRControlWidget::updateStepSize);
    
    
    updateRotationY();
    updateRotationZ();
    updateZoom();
    updateDistance();
}


    
    /*
    rotYLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rotZLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoomLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    distanceLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    stepLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    */
    
    /*
    _rotYSlider->setOrientation(Qt::Horizontal);
    _rotYSlider->setRange(0, 360);
    _rotYSlider->setSingleStep(5);
    _rotYSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    //_rotYSlider->setMaximumWidth(200);
    _rotYBox->setRange(0.f, 360.f);
    _rotYBox->setSingleStep(5.f);
    _rotYBox->setDecimals(4);
    
    _rotZSlider->setOrientation(Qt::Horizontal);
    _rotZSlider->setRange(0, 360);
    _rotZSlider->setSingleStep(5);
    //_rotZSlider->setMaximumWidth(200);
    _rotZBox->setRange(0.f, 360.f);
    _rotZBox->setSingleStep(5.f);
    _rotZBox->setDecimals(4);
    
    _zoomSlider->setOrientation(Qt::Horizontal);
    _zoomSlider->setRange(0, 2000);
    _zoomSlider->setSingleStep(25);
    //_zoomSlider->setMaximumWidth(200);
    _zoomBox->setRange(0.f, 200.f);
    _zoomBox->setSingleStep(2.5f);
    _zoomBox->setDecimals(6);
    
    _distanceSlider->setOrientation(Qt::Horizontal);
    _distanceSlider->setRange(0, 1000);
    _distanceSlider->setSingleStep(5);
    //_distanceSlider->setMaximumWidth(200);
    _distanceBox->setRange(0.f, 10.f);
    _distanceBox->setSingleStep(0.05f);
    _distanceBox->setDecimals(8);
    
    _stepSlider->setOrientation(Qt::Horizontal);
    _stepSlider->setRange(0, 10000);
    _stepSlider->setSingleStep(1);
    //_stepSlider->setMaximumWidth(200);
    _stepBox->setRange(0.f, 1.f);
    _stepBox->setSingleStep(0.0001f);
    _stepBox->setDecimals(8);
    */
    
    /*
    _rotYBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _rotZBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _zoomBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _distanceBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _stepBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    */
    
    /*
    _layout->addWidget(rotYLabel, 0, 0);
    _layout->addWidget(_rotYSlider, 0, 1);
    _layout->addWidget(_rotYBox, 0, 2);
    
    _layout->addWidget(rotZLabel, 1, 0);
    _layout->addWidget(_rotZSlider, 1, 1);
    _layout->addWidget(_rotZBox, 1, 2);
    
    _layout->addWidget(zoomLabel, 2, 0);
    _layout->addWidget(_zoomSlider, 2, 1);
    _layout->addWidget(_zoomBox, 2, 2);
    
    _layout->addWidget(distanceLabel, 3, 0);
    _layout->addWidget(_distanceSlider, 3, 1);
    _layout->addWidget(_distanceBox, 3, 2);
    
    _layout->addWidget(stepLabel, 4, 0);
    _layout->addWidget(_stepSlider, 4, 1);
    _layout->addWidget(_stepBox, 4, 2);
    */
    
    /*
    BoxSlider* _1 = new BoxSlider(1);
    BoxSlider* _2 = new BoxSlider(6);
    BoxSlider* _3 = new BoxSlider(8);
    
    QLabel* label = new QLabel("hallo");
    
    _layout->addWidget(label, 5, 0);
    _layout->addWidget(label, 6, 0);
    _layout->addWidget(label, 7, 0);
    _layout->addWidget(_1, 5, 1, 1, 2);
    _layout->addWidget(_2, 6, 1, 1, 2);
    _layout->addWidget(_3, 7, 1, 1, 2);
    */


void DVRControlWidget::updateRotationY()
{ _rotY->changedValue(_model.getRotationYDeg()); }
void DVRControlWidget::updateRotationZ()
{ _rotZ->changedValue(_model.getRotationZDeg()); }
void DVRControlWidget::updateZoom()
{ _zoom->changedValue(_model.getZoom()); }
void DVRControlWidget::updateDistance()
{ _distance->changedValue(_model.getDistance()); }
 void DVRControlWidget::updateStepSize()
{ _step->changedValue(_model.stepSize());}