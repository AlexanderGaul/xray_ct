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
    _rotGlobalZ{0.f, Eigen::Vector3f::UnitZ()},
    _rotLocalY{0.f, Eigen::Vector3f::UnitY()},
    _rot{Eigen::AngleAxisf::Identity()}
{
    _normal = point - center;
    _normal.normalize();
    _normalRef = _normal;
}


float Pose::getRotationGlobalZ() const
{
    return _rotGlobalZ.angle();
}
float Pose::getRotationLocalY() const
{
    return _rotLocalY.angle();
}
Eigen::Matrix3f Pose::getRotation() const
{
    return _rot.toRotationMatrix();
}

void Pose::setRotation(float rotGlobalZ, float rotLocalY)
{
    _rotGlobalZ = Eigen::AngleAxisf(rotGlobalZ, Eigen::Vector3f::UnitZ());
    _rotLocalY = Eigen::AngleAxisf(rotLocalY, Eigen::Vector3f::UnitY());
    _rot = _rotGlobalZ * _rotLocalY;
    updatePose();
}
void Pose::setRotationGlobalZ(float globalZ)
{
    _rotGlobalZ = Eigen::AngleAxisf(globalZ, Eigen::Vector3f::UnitZ());
    _rot = _rotGlobalZ * _rotLocalY;
    updatePose();
}
void Pose::setRotationLocalY(float localY)
{
    _rotLocalY = Eigen::AngleAxisf(localY, Eigen::Vector3f::UnitY());
    _rot = _rotGlobalZ * _rotLocalY;
    updatePose();
}
void Pose::addRotationGlobalZ(float rotZ)
{
    _rotGlobalZ = Eigen::AngleAxisf(rotZ + getRotationGlobalZ(), Eigen::Vector3f::UnitZ());
    _rot = _rotGlobalZ * _rotLocalY;
    updatePose();
}
void Pose::addRotationLocalY(float rotY)
{
    _rotLocalY = Eigen::AngleAxisf(rotY + getRotationLocalY(), Eigen::Vector3f::UnitY());
    _rot = _rotGlobalZ * _rotLocalY;
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
