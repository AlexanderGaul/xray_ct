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
            direction(i) *= 2*size;
        }
        return center + direction;
    }

    Position normalize(Position x)
    {
        float sum = 0.0;
        for(int i = 0; i<x.rows(); ++i)
        {
            sum += std::abs(x(i));
        }
        if(sum > 0)
        {
            for(int i = 0; i<x.rows(); ++i)
            {
                x(i) /= sum;
            }
        }
        return x;
    }

public:
    DVRWidget(const VisualizationModel& visModel)
        : _visModel {visModel},
          _dvrModel {M_PI, 5, calculateCameraPosition(_visModel.volume())}
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

        const Volume& vol = _visModel.volume();
        Eigen::AlignedBox3f box = vol.getBoundingBox();

        float angle = _dvrModel.angle();
        int resolution = _dvrModel.resolution();
        int steps = 100;
        int tileWidth = std::min(width(), height()) / resolution;

        // get direction vector between volume center and detector center
        Direction direction(std::cos(angle),std::sin(angle),0);
        // normalize direction
        direction = normalize(direction);
        // compute direction vector between detector and volume
        Direction correction = direction;
        // small negative values cause numerical instability...
        for(int i = 0; i<3; ++i)
        {
            if(std::abs(correction(i)) > 0.0001)
            {
                correction(i) = -correction(i);
            }
        }
        // get physical size of one pixel
        float sizePixelX = vol.getNumVoxels()[0] *
                        vol.getSpacing()[0] / resolution;
        float sizePixelZ = vol.getNumVoxels()[2] *
                        vol.getSpacing()[2] / resolution;

        // calculate position of pixel most down and left in camera
        Position better = _dvrModel.position();
        better(0) -= std::sin(angle)*sizePixelX*0.5*resolution;
        better(1) -= std::cos(angle)*sizePixelX*0.5*resolution;
        better(2) = box.corner(Eigen::AlignedBox3f::BottomLeftFloor)(2);

        std::cout << "##########" << std::endl;

        for(int i = 0; i<=resolution; ++i)
        {
            Position tmp = better + i * sizePixelZ * Eigen::Vector3f(0, 0, std::cos(angle));
            for(int j = 0; j<=resolution; ++j)
            {
                Line3f ray(tmp, correction);

                // intersection between current pixel and volume
                Eigen::Vector3f intersect = RayTracing::boxIntersect(box, ray);
                std::cout << tmp(0) << ","<<tmp(1)<<","<<tmp(2)<<std::endl;

                // do MIP (maximum intensity projection)
                float max = 0.0;
                // NaN check (NaN are odd in comparison and will not execute inner block)
                if(intersect(0) == intersect(0))
                {
                    for(int i = 0; i<steps; ++i)
                    {
                        // calculate intensity using trilinear interpolation
                        float intensity = vol.getVoxelLinearPhysical(intersect);
                        if(intensity > max)
                        {
                            max = intensity;
                        }
                        intersect += (sizePixelX/steps) * correction;
                    }
                    std::cout << max << std::endl;
                }
                // paint measured volume
                QColor color = _visModel.transferFunction().classify(max);
                painter.fillRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth, color);
                // update pixel position
                tmp += sizePixelX * Eigen::Vector3f(-std::sin(angle), std::cos(angle), 0);
            }
        }
    }

    void setAngle(float angle)
    {
        if(angle != _dvrModel.angle())
        {
            _dvrModel.setAngle(angle);
            calibrateCamera();
            repaint();
        }
    }

    void calibrateCamera()
    {
        _dvrModel.setPosition(calculateCameraPosition(_visModel.volume()));
    }
};
