#pragma once

#include <algorithm>
#include <iostream>

#include <QObject>
#include <QPainter>
#include <QWidget>

#include <QKeyEvent>

#include "DVRCamera.h"
#include "DVRModel.h"
#include "MIP.h"
#include "RayTracing.h"
#include "VisualizationModel.h"
#include "Volume.h"
#include "CameraPose.h"

/**
 * Implementation of orthographic and perspective
 * direct volume rendering (DVR).
 *
 * @brief The DVRWidget class
 */
class DVRWidget : public QWidget
{
    Q_OBJECT
private:
    VisualizationModel& _visModel;
    DVRModel& _dvrModel;
    

public:
    virtual void keyPressEvent(QKeyEvent *event) override;
    
    
    DVRWidget(VisualizationModel& visModel);

    void paintEvent(QPaintEvent* p_e) override;

    void setStepSize(float stepSize);

    void setColorRange(float from, float to);
    
    // Changes the transfer function to accomodate for the maximum intensity of the volume
    void updateVolume(Volume vol);
    
    // used to force the aspect ratio of the widget to the aspect ratio of the camera
    virtual int heightForWidth(int w) const override;
    virtual bool hasHeightForWidth() const override;
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent *event) override;
    
public slots:
    void changedPose();
    
};
