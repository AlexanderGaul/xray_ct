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
 * Implementation of orthographic
 * direct volume rendering (DVR).
 *
 * Currently only one axis roation (X value stays fixed)!
 * @brief The DVRWidget class
 */
class DVRWidget : public QWidget
{
    Q_OBJECT
private:
    VisualizationModel& _visModel;
    DVRModel& _dvrModel;
    

public:
    virtual void keyPressEvent(QKeyEvent *event);
    
    
    DVRWidget(VisualizationModel& visModel);

    void paintEvent(QPaintEvent* p_e) override;

    void setColor(QColor color);

    QColor color() const;


    void setStepSize(float stepSize);

    void setColorRange(float from, float to);
    
public slots:
    void changedPose();
    
    /*
    QSize sizeHint()
    {
        //setGeometry(0, 0, _dvrModel.getCameraPose().getPixelHorizontal(), _dvrModel.getCameraPose().getPixelHorizontal());
        return QSize(_dvrModel.getCameraPose().getPixelHorizontal(), _dvrModel.getCameraPose().getPixelHorizontal());
    }
    QSize minimumSizeHint()
    {
        //setGeometry(0, 0, _dvrModel.getCameraPose().getPixelHorizontal(), _dvrModel.getCameraPose().getPixelHorizontal());
        return QSize(_dvrModel.getCameraPose().getPixelHorizontal(), _dvrModel.getCameraPose().getPixelHorizontal());
    }
    */
};
