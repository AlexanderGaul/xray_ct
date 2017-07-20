#include "Pose.h"

Pose::Pose() 
    :
    Pose(Eigen::Vector3f{-1, 0, 0})
{}

Pose::Pose(Eigen::Vector3f point, Eigen::Vector3f center)
    :
    _pointRef{point - center},
    _point{point},
    _center{center},
    _rotZ{0.f, Eigen::Vector3f::UnitZ()},
    _rotY{0.f, Eigen::Vector3f::UnitY()},
    _rot{Eigen::AngleAxisf::Identity()}
{
    _normal = point - center;
    _normal.normalize();
    _normalRef = _normal;
}


float Pose::getRotationZ() const
{
    return _rotZ.angle();
}
float Pose::getRotationY() const
{
    return _rotY.angle();
}
Eigen::Matrix3f Pose::getRotation() const
{
    return _rot.toRotationMatrix();
}

void Pose::setRotation(float z, float y)
{
    _rotZ = Eigen::AngleAxisf(z, Eigen::Vector3f::UnitZ());
    _rotY = Eigen::AngleAxisf(y, Eigen::Vector3f::UnitY());
    _rot = _rotZ * _rotY;
    updatePose();
}
void Pose::setRotationZ(float z)
{
    _rotZ = Eigen::AngleAxisf(z, Eigen::Vector3f::UnitZ());
    _rot = _rotZ * _rotY;
    updatePose();
}
void Pose::setRotationY(float y)
{
    _rotY = Eigen::AngleAxisf(y, Eigen::Vector3f::UnitY());
    _rot = _rotZ * _rotY;
    updatePose();
}
void Pose::addRotationZ(float rotZ)
{
    _rotZ = Eigen::AngleAxisf(rotZ + getRotationZ(), Eigen::Vector3f::UnitZ());
    _rot = _rotZ * _rotY;
    updatePose();
}
void Pose::addRotationY(float rotY)
{
    _rotY = Eigen::AngleAxisf(rotY + getRotationY(), Eigen::Vector3f::UnitY());
    _rot = _rotZ * _rotY;
    updatePose();
}


Eigen::Vector3f Pose::getCenter() const
{
    return _center;
}
void Pose::setCenter(Eigen::Vector3f center)
{
    _center = center;
    updatePose();
}

Eigen::Vector3f Pose::getPoint() const
{
    return _point;
}
void Pose::setPointRef(Eigen::Vector3f point)
{
    _pointRef = point;
    _normalRef = _pointRef;
    _normalRef.normalize();
    updatePose();
}
Eigen::Vector3f Pose::getNormal() const
{
    return _normal;
}

void Pose::setDistanceToCenter(float distance)
{
    _pointRef.normalize();
    _pointRef = _pointRef * distance;
    updatePose();
}


void Pose::updatePose()
{
    _point = getRotation() * _pointRef + _center;
    _normal = getRotation() * _normalRef;
}
