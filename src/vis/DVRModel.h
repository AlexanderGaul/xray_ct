#ifndef DVRMODEL_H
#define DVRMODEL_H

#include <Eigen/Eigen>
#include <QObject>
#include <QWidget>

#include "TransferFunction.h"
#include "CameraPose.h"
//#include "VisualizationModel.h"


class DVRModel : public QObject
{
    Q_OBJECT
private:
    float _stepSize;
    bool _perspective;
    TransferFunction _transferFunction;
    QColor _color;
    
    CameraPose _pose;
    
    //VisualizationModel& _model;
    
public:
    DVRModel(float stepSize, TransferFunction transferFunction)
        :
          _stepSize{stepSize},
          _perspective{false},
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
        emit redraw();
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
    
signals:
    void redraw();
    
    void updateRotationY();
    void updateRotationZ();
    void updateZoom();
    void updateDistance();
    void updateStepSize();
    
public slots:
    void setRotationYDeg(float y)
    {
        float radians = y / 180.f * M_PI;
        _pose.setRotationY(radians);
        emit redraw();
    }
    void setRotationZDeg(float z)
    {
        float radians = z / 180.f * M_PI;
        _pose.setRotationZ(radians);
        emit redraw();
    }
    void setDistance(float distance)
    {
        _pose.setDistance(distance);
        emit redraw();
    }
    void setZoom(float zoom)
    {
        _pose.setFov(zoom);
        emit redraw();
    }
    void setPerspective(bool persp)
    {
        _perspective = persp;
        emit redraw();
    }
    
public:
    float getRotationYDeg()
    {
        float deg = _pose.getRotationY() / M_PI * 180.f;
        return deg;
    }
    
    float getRotationZDeg()
    {
        float deg = _pose.getRotationZ() / M_PI * 180.f;
        return deg;
    }
    
    float getDistance()
    {
        return _pose.getDistance();
    }
    
    float getZoom()
    {
        return _pose.getFov();
    }
    
    bool getPerspective()
    {
        return _perspective;
    }
    
};

#endif // DVRMODEL_H
