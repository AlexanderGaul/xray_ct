#pragma once

#include <iostream>

#include <QObject>
#include <QPainter>
#include <QPen>
#include <QWidget>

#include "VisualizationModel.h"

class MPRWidget : public QWidget
{
    Q_OBJECT

private:
    const VisualizationModel& _model;

public:
    MPRWidget(const VisualizationModel& model)
        : _model(model)
    {
    }

    void paintEvent(QPaintEvent* p_e)
    {
        QPainter painter(this);
        QPen pen;
        pen.setColor(QColor::fromRgb(0, 0, 0));
        painter.setPen(pen);

        Eigen::Vector3f t1(0.0, 0.0, 0.0);
        Eigen::Vector3f t2(0.0, 0.0, 9.0);
        Eigen::Vector3f t3(0.0, 9.0, 9.0);

        int steps = 50;
        int tileWidth = 10;

        Eigen::Vector3f d1 = t2-t1;
        d1 /= steps;
        Eigen::Vector3f d2 = t3-t1;
        d2 /= steps;

        for(int i = 0; i<=steps; ++i)
        {
            Eigen::Vector3f curr = t1 + d2 * i;
            for(int j = 0; j<=steps; ++j)
            {
                float intensity = _model.volume().getVoxelLinear(curr);
                QColor color = _model.transferFunction().classify(intensity);
                pen.setColor(color);
                painter.fillRect(QRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth), Qt::SolidPattern);
                curr += d1;
            }
        }
    }
};
