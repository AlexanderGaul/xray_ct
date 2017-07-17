#pragma once

#include <iostream>

#include <QObject>
#include <QPainter>
#include <QPen>
#include <QWidget>

#include "MPRModel.h"
#include "VisualizationModel.h"

class MPRWidget : public QWidget
{
    Q_OBJECT

private:
    const VisualizationModel& _visModel;
    MPRModel _mprModel;

public:
    /**
     * Creates a new MPRWidget and sets up
     * the corresponding model classes.
     * Some data specific for MPR is contained inside
     * of the special MPRModel.
     * @brief MPRWidget
     * @param visModel
     */
    MPRWidget(const VisualizationModel& visModel);

    /**
     * This method is called to paint the MPR on the
     * screen, every time the size of the window changes
     * or the repaint() method was called.
     * @brief paintEvent
     * @param p_e - PaintEvent, not used for MPR
     */
    void paintEvent(QPaintEvent* p_e) override;

    void updateT4()
    {
        Eigen::Vector3f t4 = _mprModel.t1() + (_mprModel.t2() - _mprModel.t1()) + (_mprModel.t3() - _mprModel.t1());
        _mprModel.setT4(t4);
    }

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
     * Sets the first point describing the 2D plane of MPR.
     * @brief setT1
     * @param position - position of the first point
     */
    void setT1(Eigen::Vector3f position);

    /**
     * Sets the second point describing the 2D plane of MPR.
     * @brief setT2
     * @param position - position of the second point
     */
    void setT2(Eigen::Vector3f position);

    /**
     * Sets the third point describing the 2D plane of MPR.
     * @brief setT3
     * @param position - position of the third point
     */
    void setT3(Eigen::Vector3f position);

    Eigen::Vector3f t1() const
    {
        return _mprModel.t1();
    }

    Eigen::Vector3f t2() const
    {
        return _mprModel.t2();
    }

    Eigen::Vector3f t3() const
    {
        return _mprModel.t3();
    }

    Eigen::Vector3f t4() const
    {
        return _mprModel.t4();
    }
};
