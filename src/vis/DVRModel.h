#pragma once
#ifndef DVRMODEL_H
#define DVRMODEL_H

#include <Eigen/Eigen>

#include "TransferFunction.h"


class DVRModel
{
private:
    /// specifies the angle of camera in radians
    float _angle;
    /// specifies the camera resolution (camera has
    /// resolution x resolution pixels)
    int _resolution;
    /// specifies the position of the camera
    Eigen::Vector3f _position;
    float _stepSize;

    TransferFunction _transferFunction;

    QColor _color;
    
public:
    DVRModel(float angle, int resolution, Eigen::Vector3f position, float stepSize,
             TransferFunction transferFunction)
        : _angle(angle),
          _resolution(resolution),
          _position {position},
          _stepSize {stepSize},
          _transferFunction {transferFunction},
          _color(transferFunction.color(0))
    {
    }


    float angle() const
    {
        return _angle;
    }

    void setAngle(float angle)
    {
        _angle = angle;
    }

    int resolution() const
    {
        return _resolution;
    }

    Eigen::Vector3f position() const
    {
        return _position;
    }

    void setPosition(Eigen::Vector3f position)
    {
        _position = position;
    }

    void setTransferFunction(TransferFunction transferFunction)
    {
        _transferFunction = transferFunction;
    }

    const TransferFunction& transferFunction() const
    {
        return _transferFunction;
    }


    void setColor(QColor color)
    {
        _color = color;
        _transferFunction.setColor(0, color);
    }

    const QColor& color() const
    {
        return _color;
    }

    void setStepSize(float stepsize)
    {
        _stepSize = stepsize;
    }

    float stepSize() const
    {
        return _stepSize;
    }

    void setColorRange(float from, float to)
    {
        _transferFunction.setRange(0, from, to);
    }
};

#endif // DVRMODEL_H
