#pragma once
#ifndef MPRMODEL_H
#define MPRMODEL_H

#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <QObject>

#include "TransferFunction.h"
#include "CameraPose.h"

typedef Eigen::Vector3f Position;

/**
 * The MPRModel contains all data needed only for the
 * MPR visualization process.
 * @brief The MPRModel class
 */
class MPRModel : public QObject
{
    Q_OBJECT
private:
    int _granularity;
    float _distance;
    Eigen::Vector3f _normal;
    TransferFunction _transferFunction;
    
    CameraPose _pose;
    
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
    
    
    int getPixelHorizontal();
    int getPixelVertical();
    
    Eigen::Vector3f getPixel(int horizontal, int vertical);
    
    
    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    
    void setRotationYDeg(float y);
    void setRotationZDeg(float z);
    
    void setZoom(float zoom);
    
    float getPositionX();
    float getPositionY();
    float getPositionZ();
    
    float getRotationYDeg();
    float getRotationZDeg();
    
    float getZoom();
    

signals:
    void redraw();
    
    
};

#endif //MPRMODEL_H
