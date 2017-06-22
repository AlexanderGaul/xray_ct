#include "AcquisitionPose.h"

AcquisitionPose::AcquisitionPose() :
    AcquisitionPose(0.5f, 0.2f, 0.2f, 5, 5)
{
}

AcquisitionPose::AcquisitionPose(float s2dd, float det_w, float det_h, int pixel_h, int pixel_v) :
    s2dd_(s2dd),
    src_(- s2dd / 2.f, 0.f, 0.f), 
    det_(s2dd / 2.f, 0.f, 0.f),
    det_normal_(-1.f, 0.f, 0.f),
    det_upl_(s2dd/2.f, det_w/2.f, det_h/2.f),
    det_width_(det_w), 
    det_height_(det_h), 
    pxl_horizontal_(pixel_h), 
    pxl_vertical_(pixel_v),
    center_(0.f, 0.f, 0.f),
    rot_global_z_(0.f, Eigen::Vector3f(0.f, 0.f, 1.f)),
    rot_local_y_(0.f, Eigen::Vector3f(0.f, 1.f, 0.f))
{
}

Eigen::Vector3f AcquisitionPose::getSourcePosition()
{
    return src_ ;
}

Eigen::Vector3f AcquisitionPose::getDetectorCenter()
{
    return det_;
}
float AcquisitionPose::getDetectorWidth()
{ return det_width_;}
Eigen::Vector3f AcquisitionPose::getCenter()
{ return center_;}

float AcquisitionPose::getRotationGlobalZ()
{ return rot_global_z_.angle(); }
float AcquisitionPose::getRotationLocalY()
{ return rot_local_y_.angle(); }

Eigen::Matrix3f AcquisitionPose::getRot()
{
    Eigen::Matrix3f m1 = rot_global_z_.toRotationMatrix();
    Eigen::Matrix3f m2 =  rot_local_y_.toRotationMatrix();
    return m2 * m1;
}

int AcquisitionPose::getPixelHorizontal()
{ return pxl_horizontal_; }
int AcquisitionPose::getPixelVertical()
{ return pxl_vertical_; }


void AcquisitionPose::updatePose()
{
    src_        = getRot() * Eigen::Vector3f(- s2dd_ / 2.f, 0.f, 0.f) + center_;
    det_        = getRot() * Eigen::Vector3f(s2dd_ / 2.f, 0.f, 0.f) + center_;
    det_normal_ = getRot() * Eigen::Vector3f(-1.f, 0.f, 0.f);
    det_upl_    = getRot() * Eigen::Vector3f(s2dd_ / 2.f, det_width_ / 2.f, det_height_ / 2.f) + center_;
}

void AcquisitionPose::setCenter(const Eigen::Vector3f& center)
{
    center_ = center;
    updatePose();
}
void AcquisitionPose::setRotation(float rot_global_z, float rot_local_y)
{
    rot_global_z_ = Eigen::AngleAxisf(rot_global_z, Eigen::Vector3f(0.f, 0.f, 1.f));
    rot_local_y_ = Eigen::AngleAxisf(rot_local_y, rot_global_z_ * Eigen::Vector3f(0.f, 1.f, 0.f));
    updatePose();
}
void AcquisitionPose::setRotationGlobalZ(float rot_global_z)
{
    setRotation(rot_global_z, rot_local_y_.angle());
}
void AcquisitionPose::setRotationLocalY(float rot_local_y)
{
    rot_local_y_ = Eigen::AngleAxisf(rot_local_y, rot_global_z_ * Eigen::Vector3f(0.f, 1.f, 0.f));
    updatePose();   
}

Eigen::Vector3f AcquisitionPose::getDetectorUpperLeft()
{
    return det_upl_;
}
Eigen::Vector3f AcquisitionPose::getDetectorLowerLeft()
{
    return getRot() * Eigen::Vector3f(s2dd_ / 2.f, det_width_ / 2.f, -det_height_ / 2.f) + center_;
}
Eigen::Vector3f AcquisitionPose::getDetectorLowerRight()
{
    return getRot() * Eigen::Vector3f(s2dd_ / 2.f, -det_width_ / 2.f, -det_height_ / 2.f) + center_;
}
Eigen::Vector3f AcquisitionPose::getDetectorUpperRight()
{
    return getRot() * Eigen::Vector3f(s2dd_ / 2.f, -det_width_ / 2.f, det_height_ / 2.f) + center_;
}

Eigen::ParametrizedLine<float, 3> AcquisitionPose::getRay(int horizontal, int vertical)
{
    Eigen::Vector3f pixel_center = getPixelCenter(horizontal, vertical);

    Eigen::Vector3f dir = pixel_center - getSourcePosition();
    dir.normalize();

    return Eigen::ParametrizedLine<float, 3>(getSourcePosition(), dir);
}

Eigen::Vector3f AcquisitionPose::getPixelCenter(int horizontal, int vertical)
{
    Eigen::Vector3f right = getRot() * Eigen::Vector3f(0.f, -1.f, 0.f);
    Eigen::Vector3f down = getRot() * Eigen::Vector3f(0.f, 0.f, -1.f);

    Eigen::Vector3f pxl_dist_h = right * (det_width_ / pxl_horizontal_);
    Eigen::Vector3f pxl_dist_v = down * (det_height_ / pxl_vertical_);

    Eigen::Vector3f pixel_center = getDetectorUpperLeft() + pxl_dist_h * (1. / 2.) + pxl_dist_h * horizontal + pxl_dist_v * (1. / 2.) + pxl_dist_v * vertical;

    return pixel_center;
}