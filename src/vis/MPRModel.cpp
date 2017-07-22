#include "MPRModel.h"

MPRModel::MPRModel()
    : _granularity(10),
      _distance(0.01),
      _normal(1,0,0),
      _transferFunction(TransferFunction(LinearPiece(0, 100, 0, 255, QColor::fromRgb(255,255,255)))),
      _color(QColor::fromRgb(255,255,255)),
      _pose(200, 200, 0.f)
{
}

MPRModel::MPRModel(int granularity, float distance, Eigen::Vector3f normal,
                   TransferFunction transferFunction)
    : _granularity(granularity),
      _distance(distance),
      _transferFunction(transferFunction),
      _color(transferFunction.color(0))
{
}

int MPRModel::granularity() const
{
    return _granularity;
}

void MPRModel::setGranularity(int granularity)
{
    _granularity = granularity;
}



int MPRModel::getPixelHorizontal()
{
    return _pose.getPixelHorizontal();
}
int MPRModel::getPixelVertical()
{
    return _pose.getPixelHorizontal();
}

Eigen::Vector3f MPRModel::getPixel(int horizontal, int vertical)
{
    return _pose.getPixel(horizontal, vertical);
}



void MPRModel::setZoom(float zoom)
{
    _pose.setFov(zoom);
    emit redraw();
}
void MPRModel::setPositionX(float x)
{
    Eigen::Vector3f center = _pose.getCenter();
    _pose.setCenter(Eigen::Vector3f(x, center.y(), center.z()));
    emit redraw();
}
void MPRModel::setPositionY(float y)
{
    Eigen::Vector3f center = _pose.getCenter();
    _pose.setCenter(Eigen::Vector3f(center.x(), y, center.z()));
    emit redraw();
}
void MPRModel::setPositionZ(float z)
{
    Eigen::Vector3f center = _pose.getCenter();
    _pose.setCenter(Eigen::Vector3f(center.x(), center.y(), z));
    emit redraw();
}
void MPRModel::setRotationYDeg(float y)
{
    float radians = y / 180.f * M_PI;
    _pose.setRotationY(radians);
    emit redraw();
}
void MPRModel::setRotationZDeg(float z)
{
    float radians = z / 180.f * M_PI;
    _pose.setRotationZ(radians);
    emit redraw();
}


float MPRModel::getPositionX()
{
    return _pose.getCenter().x();
}
float MPRModel::getPositionY()
{
    return _pose.getCenter().y();
}
float MPRModel::getPositionZ()
{
    return _pose.getCenter().z();
}

float MPRModel::getRotationYDeg()
{
    float deg = _pose.getRotationY() / M_PI * 180.f;
    return deg;
}
float MPRModel::getRotationZDeg()
{
    float deg = _pose.getRotationZ() / M_PI * 180.f;
    return deg;
}

float MPRModel::getZoom()
{
    return _pose.getFov();
}