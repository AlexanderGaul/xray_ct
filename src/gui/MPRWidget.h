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
    MPRWidget(const VisualizationModel& visModel)
        : _visModel {visModel},
          _mprModel {}
    {
        resize(500, 500);
    }

    void paintEvent(QPaintEvent* p_e)
    {
        if(_visModel.volume().getTotalVoxelCount() == 0)
        {
            return; // no data yet
        }
        int steps = _mprModel.granularity();
        QPainter painter(this);
        // set background
        painter.fillRect(0,0, width(), height(), Qt::black);

        int tileWidth = std::min(width(), height());
        tileWidth /= steps;

        Eigen::Vector3f d1 = _mprModel.t2() - _mprModel.t1();
        d1 /= (steps);
        Eigen::Vector3f d2 = _mprModel.t3() - _mprModel.t2();
        d2 /= (steps);

        for(int i = 0; i<steps; ++i)
        {
            Eigen::Vector3f curr = _mprModel.t1() + d2 * i;
            for(int j = 0; j<steps; ++j)
            {
                float intensity = _visModel.volume().getVoxelLinear(curr);
                if(intensity < 0) intensity = 0;
                QColor color = _visModel.transferFunction().classify(intensity);

                painter.fillRect(QRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth), color);
                curr += d1;
            }
        }
    }

public slots:
    void setGranularity(int granularity)
    {
        _mprModel.setGranularity(granularity);
    }

    void setT1(Eigen::Vector3f position)
    {
        _mprModel.setT1(position);
    }

    void setT2(Eigen::Vector3f position)
    {
        _mprModel.setT2(position);
    }

    void setT3(Eigen::Vector3f position)
    {
        _mprModel.setT3(position);
    }
};
