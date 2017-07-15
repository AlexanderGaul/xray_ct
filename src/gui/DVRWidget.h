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
    enum class Mode
    {
        Empty,
        Filled
    };

    using Position=Eigen::Vector3f;
    using Direction=Eigen::Vector3f;

    int EMPTY = 0;
    int FILLED = 1;

    const VisualizationModel& _visModel;
    DVRModel _dvrModel;
    int _mode;

    Position calculateCameraPosition(const Volume& volume)
    {
        if(volume.getTotalVoxelCount() == 0)
        {
            _mode = EMPTY;
            return Position(0,0,0); // volume was empty
        }
        else
        {
            _mode = FILLED;
        }
        Eigen::AlignedBox3f box = volume.getBoundingBox();
        Position blf = box.corner(Eigen::AlignedBox3f::BottomLeftFloor);
        Position trc = box.corner(Eigen::AlignedBox3f::TopRightCeil);

        Position center(3);
        for(int i = 0; i<3; ++i)
        {
            float min = std::min(blf(i), trc(i));
            float delta = std::abs(blf(i)-trc(i));
            center(i) = min + (delta*0.5);
        }

        Direction direction(0,0,0);
        direction(0) += std::cos(_dvrModel.angle());
        direction(1) += std::sin(_dvrModel.angle());
        float size = volume.getNumVoxels()[0] *
                        volume.getSpacing()[0];
        for(int i = 0; i<3; ++i)
        {
            direction(i) *= 1.5*size;
        }
        return center + direction;
    }

public:
    DVRWidget(const VisualizationModel& visModel)
        : _visModel {visModel},
          _dvrModel {0.0, 5, calculateCameraPosition(_visModel.volume())}
    {
    }

    void paintEvent(QPaintEvent* p_e) override
    {
        if(_mode == EMPTY)
        {
            return;
        }
        QPainter painter(this);
        painter.fillRect(0, 0, width(), height(), QColor::fromRgb(0,0,0));
        Eigen::AlignedBox3f box = _visModel.volume().getBoundingBox();

        // get direction
        Direction direction(std::cos(_dvrModel.angle()),std::sin(_dvrModel.angle()),0);
        // normalize direction
        float sum = 0.0;
        for(int i = 0; i<direction.rows(); ++i)
        {
            sum += std::abs(direction(i));
        }
        if(sum > 0)
        {
            for(int i = 0; i<direction.rows(); ++i)
            {
                direction(i) /= sum;
            }
        }
        Position pos = _dvrModel.position();
        Direction correction = Direction(-direction(0), direction(1), 0);
        float size = _visModel.volume().getNumVoxels()[0] *
                        _visModel.volume().getSpacing()[0];
        Position better = pos-(correction*size*0.5);
        Line3f ray(better, direction);

        int steps = 100;
        float stepSize = size/steps;
        int tileWidth = 10;
        for(int i = 0; i<_dvrModel.resolution(); ++i)
        {
            better += correction;
            Eigen::Vector3f intersect = RayTracing::boxIntersect(box, ray);
            for(int j = 0; j<_dvrModel.resolution(); ++j)
            {
                float max = 0.0;
                for(int k = 0; k<steps; ++k)
                {
                    float value = _visModel.volume().getVoxelLinear(intersect);
                    if(value == -1) break; //end of volume reached

                    max = value > max ? value : max;
                    intersect += stepSize * direction;
                }
                painter.fillRect(QRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth),
                                 _visModel.transferFunction().classify(max));
                std::cout << max << std::endl;
            }
        }
    }

    void setAngle(float alpha)
    {
        _dvrModel.setAngle(alpha);
    }

    void calibrateCamera()
    {
        _dvrModel.setPosition(calculateCameraPosition(_visModel.volume()));
    }
};
