#include "AcquisitionPose.h"


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
    rotation_(0.f, Eigen::Vector3f(0.f, 0.f, 1.f))  
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

void AcquisitionPose::updatePose()
{
    src_ = rotation_ * Eigen::Vector3f(- s2dd_ / 2.f, 0.f, 0.f) + center_;
    det_ = rotation_ * Eigen::Vector3f(s2dd_ / 2.f, 0.f, 0.f) + center_;
    det_normal_ = rotation_ * Eigen::Vector3f(-1.f, 0.f, 0.f);
    det_upl_ = rotation_ * Eigen::Vector3f(s2dd_ / 2.f, det_width_ / 2.f, det_height_ / 2.f) + center_;
}

void AcquisitionPose::setCenter(Eigen::Vector3f center)
{
    center_ = center;
    updatePose();
}
void AcquisitionPose::setRotation(float rotation)
{
    rotation_ = Eigen::AngleAxis<float>(rotation, Eigen::Vector3f(0.f, 0.f, 1.f));
    updatePose();
}

Eigen::Vector3f AcquisitionPose::getDetectorUpperLeft()
{
    return det_upl_;
}
Eigen::Vector3f AcquisitionPose::getDetectorLowerRight()
{
    return rotation_ * Eigen::Vector3f(s2dd_ / 2.f, -det_width_ / 2.f, -det_height_ / 2.f) + center_;
}

Eigen::ParametrizedLine<float, 3> AcquisitionPose::getRay(int horizontal, int vertical)
{
    Eigen::Vector3f right = rotation_ * Eigen::Vector3f(0.f, -1.f, 0.f);
    Eigen::Vector3f down = rotation_ * Eigen::Vector3f(0.f, 0.f, -1.f);

    Eigen::Vector3f pxl_dist_h = right * (det_width_ / pxl_horizontal_);
    Eigen::Vector3f pxl_dist_v = down * (det_height_ / pxl_vertical_);

    Eigen::Vector3f pixel_center = getDetectorUpperLeft() + pxl_dist_h * (1. / 2.) + pxl_dist_h * horizontal + pxl_dist_v * (1. / 2.) + pxl_dist_v * vertical;

    Eigen::Vector3f dir = pixel_center - getSourcePosition();
    dir.normalize();

    return Eigen::ParametrizedLine<float, 3>(getSourcePosition(), dir);
}
