#pragma once
#ifndef DVRMODEL_H
#define DVRMODEL_H

#include <Eigen/Eigen>

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
public:
    DVRModel(float angle, int resolution, Eigen::Vector3f position)
        : _angle(angle),
          _resolution(resolution),
          _position {position}
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
};

#endif // DVRMODEL_H
