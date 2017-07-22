#pragma once

#include <iostream>

#include <QObject>
#include <QPainter>
#include <QPen>
#include <QWidget>

#include "MPRModel.h"
#include "RayTracing.h"
#include "VisualizationModel.h"

class MPRWidget : public QWidget
{
    Q_OBJECT

private:
    VisualizationModel& _visModel;
    MPRModel& _mprModel;

    /*
    float calculateIntersectionLength(Line3f ray)
    {
        float factor = 100;
        float epsilon = 0.000000001;
        Eigen::AlignedBox3f box = _visModel.volume().getBoundingBox();
        Eigen::Vector3f direction = ray.direction();
        Eigen::Vector3f minusDir = -direction;
        // prevent -0 values that would cause problems in ray tracing
        for(int i = 0; i<3; ++i)
        {
            if(std::abs(minusDir(i)) < epsilon)
            {
                minusDir(i) = 0;
            }
            if(std::abs(direction(i)) < epsilon)
            {
                direction(i) = 0;
            }
        }
        Eigen::Vector3f outside1 = ray.origin() + factor * direction;
        Eigen::Vector3f intersect1 = RayTracing::boxIntersect(box,
                                                              Line3f(outside1, minusDir));

        Eigen::Vector3f outside2 = ray.origin() - factor * direction;
        Eigen::Vector3f intersect2 = RayTracing::boxIntersect(box, Line3f(outside2, direction));

        float distance = 0;
        for(int i = 0; i<3; ++i)
        {
            distance += std::abs(intersect1(i)-intersect2(i));
        }
        return distance;
    }

    Eigen::Vector3f calculateOrigin(Eigen::Vector3f point, Eigen::Vector3f direction)
    {
        Eigen::AlignedBox3f box = _visModel.volume().getBoundingBox();
        int factor = 100;
        float epsilon = 0.000000001;
        Eigen::Vector3f minusDir = -direction;
        // prevent -0 values that would cause problems in ray tracing
        for(int i = 0; i<3; ++i)
        {
            if(std::abs(minusDir(i)) < epsilon)
            {
                minusDir(i) = 0;
            }
            if(std::abs(direction(i)) < epsilon)
            {
                direction(i) = 0;
            }
        }
        Eigen::Vector3f outside1 = point - factor * direction;
        Eigen::Vector3f intersect1 = RayTracing::boxIntersect(box,
                                                              Line3f(outside1, direction));
        return intersect1;
    }
    */

public:
    /**
     * Creates a new MPRWidget and sets up
     * the corresponding model classes.
     * Some data specific for MPR is contained inside
     * of the special MPRModel.
     * @brief MPRWidget
     * @param visModel
     */
    MPRWidget(VisualizationModel& visModel);

    /**
     * This method is called to paint the MPR on the
     * screen, every time the size of the window changes
     * or the repaint() method was called.
     * @brief paintEvent
     * @param p_e - PaintEvent, not used for MPR
     */
    void paintEvent(QPaintEvent* p_e) override;

public slots:
    /**
     * Sets the granularity of the mpr widget.
     * Currently, the granularity slider can be adjusted
     * to values from 1 to 10, which are then scaled to values
     * of 10, 20, ..., 100 inside this method.
     * @brief setGranularity
     * @param granularity
     */
    void setGranularity(int granularity);

    /**
     * Sets the color used for the MPR transfer
     * function visualization.
     * @brief setColor
     * @param color
     */
    void setColor(QColor color);

    /**
     * Gets the current color used for the MPR transfer
     * function visualization.
     * @brief color
     * @return
     */
    QColor color() const;

    void setDistance(float distance)
    {
        _mprModel.setDistance(distance);
        update();
    }

    void setNormal(Eigen::Vector3f normal)
    {
        _mprModel.setNormal(normal);
        update();
    }

    Eigen::Vector3f normal()
    {
        return _mprModel.normal();
    }

public slots:
    void changedPose();

};
