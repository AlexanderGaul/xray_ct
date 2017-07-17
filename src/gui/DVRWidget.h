#pragma once

#include <algorithm>
#include <iostream>

#include <QObject>
#include <QPainter>
#include <QWidget>

#include "DVRModel.h"
#include "VisualizationModel.h"
#include "RayTracing.h"
#include "Volume.h"

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
    int EMPTY = 0;
    int FILLED = 1;

    const VisualizationModel& _visModel;
    DVRModel _dvrModel;
    int _mode;

    Eigen::Vector3f calculateCameraPosition(const Volume& volume);

    Eigen::Vector3f normalize(Eigen::Vector3f x);

public:
    DVRWidget(const VisualizationModel& visModel);

    void paintEvent(QPaintEvent* p_e) override;

    /**
     * Sets the rotation angle for the DVR.
     * @brief setAngle
     * @param angle - the angle of rotation in radians
     */
    void setAngle(float angle);

    /**
     * Calibrates the camera according to the corresponding
     * volume.
     * @brief calibrateCamera
     */
    void calibrateCamera();
};
