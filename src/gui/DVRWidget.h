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
    ///internal constant defining the volume used is empty
    int EMPTY = 0;
    ///internal constant defining the volume used is not empty
    int FILLED = 1;

    const VisualizationModel& _visModel;
    DVRModel _dvrModel;
    int _mode;

    /**
     * Calculates the camera position.
     *
     * More specifically, the position of the pixel most left
     * and at the bottom of the camera is calculated.
     * @brief calculateCameraPosition
     * @param volume
     * @return
     */
    Eigen::Vector3f calculateCameraPosition(const Volume& volume);

    /**
     * Normalizes the input vector,
     * such that the l1 norm is exactly one.
     * @brief normalize
     * @param x input vector containing exactly three values
     * @return normalized vector, in the same direction
     *          as the input vector.
     */
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

    void setColor(QColor color)
    {
        _dvrModel.setColor(color);
    }

    QColor color() const
    {
        return _dvrModel.color();
    }

    void setStepSize(float stepSize)
    {
        _dvrModel.setStepSize(stepSize);
        repaint();

    }
};
