#pragma once
#ifndef MPRMODEL_H
#define MPRMODEL_H

#include <Eigen/Eigen>

#include "TransferFunction.h"

typedef Eigen::Vector3f Position;

/**
 * The MPRModel contains all data needed only for the
 * MPR visualization process.
 * @brief The MPRModel class
 */
class MPRModel
{
private:
    int _granularity;
    float _distance;
    Eigen::Vector3f _normal;
    TransferFunction _transferFunction;
    QColor _color;

public:
    MPRModel();

    MPRModel(int granularity, float distance, Eigen::Vector3f _normal,
             TransferFunction transferFunction);

    int granularity() const;

    void setGranularity(int granularity);

    void setTransferFunction(TransferFunction transferFunction)
    {
        _transferFunction = transferFunction;
    }

    TransferFunction& transferFunction()
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

    void setDistance(float distance)
    {
        _distance = distance;
    }

    float distance()
    {
        return _distance;
    }

    void setNormal(Eigen::Vector3f normal)
    {
        _normal = normal;
    }

    Eigen::Vector3f normal()
    {
        return _normal;
    }
};

#endif //MPRMODEL_H
