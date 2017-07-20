#include <math.h>

#include <iostream>

#include "CameraPose.h"



CameraPose::CameraPose()
{}


CameraPose::CameraPose(const Eigen::AlignedBox<float, 3>& boundingBox, int pixelHorizontal, int pixelVertical)
    :
    Pose(Eigen::Vector3f(-boundingBox.diagonal().norm(), 0.f, 0.f)),
    _pxlHorizontal{pixelVertical},
    _pxlVertical{pixelVertical},
    _minDistance{boundingBox.diagonal().norm() / 2.f},
    _down{0.f, 0.f, -1.f},
    _right{0.f, 1.f, 0.f},
    _screenCenter{- boundingBox.diagonal().norm() / 2.f, 0.f, 0.f}
{
    setFov(120);
}

void CameraPose::setBoundingBox(const Eigen::AlignedBox<float, 3> boundingBox)
{
    _minDistance = boundingBox.diagonal().norm() / 2.f; 
    if(getPoint().norm() == 0)
    {
        setPointRef(Eigen::Vector3f(-boundingBox.diagonal().norm() / 3.f, 0.f, 0.f));
    }
    setFov(_fov);
    updatePose();
}
void CameraPose::setFov(float fov)
{
    _fov = fov;
    float screenWidth = 2 * _minDistance * tanf(_fov / 2.f / 180.f * M_PI);
    _pixelSize = screenWidth / _pxlHorizontal;
}

int CameraPose::getPixelHorizontal() const
{
    return _pxlHorizontal;
}
int CameraPose::getPixelVertical() const
{
    return _pxlVertical;
}

Eigen::ParametrizedLine<float, 3> CameraPose::getRayProjective(int horizontal, int vertical)
{
    Eigen::Vector3f pixel = getPixel(horizontal, vertical, getCenter());
    Eigen::Vector3f direction = pixel - getPoint();
    direction.normalize();
    return Eigen::ParametrizedLine<float, 3>(getPoint(), direction);
}
Eigen::ParametrizedLine<float, 3> CameraPose::getRayOrthogonal(int horizontal, int vertical)
{
    return Eigen::ParametrizedLine<float, 3>(getPixel(horizontal, vertical, getPoint()), -getNormal());
}

void CameraPose::setDistance(float distance)
{
    Pose::setDistanceToCenter(distance);
    _screenCenter = getRotation() * (getPoint() - getNormal() * _minDistance);
}
float CameraPose::getDistance()
{
    return getPoint().norm();
}


void CameraPose::updatePose()
{
    Pose::updatePose();
    _screenCenter = getPoint() - getNormal() * _minDistance;
    _down = getRotation() * Eigen::Vector3f(0.f, 0.f, -1.f);
    _right = getRotation() * Eigen::Vector3f(0.f, 1.f, 0.f);
}

Eigen::Vector3f CameraPose::getPixel(int horizontal, int vertical)
{
    return getPixel(horizontal, vertical, getCenter());
}

Eigen::Vector3f CameraPose::getPixel(int horizontal, int vertical, Eigen::Vector3f screenCenter)
{
    Eigen::Vector3f position = screenCenter;
    
    float stepX = (_pxlHorizontal / 2.f - 0.5f) * _pixelSize;
    float stepY = (_pxlVertical / 2.f - 0.5f) * _pixelSize;
    
    position = position - stepX * _right;
    position = position - stepY * _down;
    
    position = position + _right * _pixelSize * horizontal;
    position = position + _down * _pixelSize * vertical;
    
    return position;
}