#pragma once
#ifndef DVRMODEL_H
#define DVRMODEL_H

#include <Eigen/Eigen>

#include "TransferFunction.h"
#include "CameraPose.h"


class DVRModel
{
private:
    float _stepSize;
    TransferFunction _transferFunction;
    QColor _color;
    
    CameraPose _pose;
    
public:
    DVRModel(float stepSize, TransferFunction transferFunction)
        :
          _stepSize{stepSize},
          _transferFunction {transferFunction},
          _color(transferFunction.color(0))
    {
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
    
    CameraPose& getCameraPose()
    {
        return _pose;
    }
};

#endif // DVRMODEL_H
