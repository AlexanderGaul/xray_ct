#ifndef ACQUISITIONPOSE_H
#define ACQUISITIONPOSE_H

#include <vector>
#include <Eigen/Dense>

class AcquisitionPose
{
private:
    float s2dd_;

    Eigen::Vector3f src_;
    Eigen::Vector3f det_;
    Eigen::Vector3f det_normal_;
    Eigen::Vector3f det_upl_;

    Eigen::Vector3f center_;
    Eigen::AngleAxis<float> rotation_;

    float det_width_;
    float det_height_;

    int pxl_horizontal_;
    int pxl_vertical_;

    void updatePose();

    Eigen::Vector3f getDetectorLeft();
    Eigen::Vector3f getDetectorRight();

    Eigen::Vector3f getDetectorUpperLeft();
    Eigen::Vector3f getDetectorLowerRight();

public:
    AcquisitionPose(float s2dd, float det_w, float det_h, int pixel_h, int pixel_v);

    Eigen::Vector3f getSourcePosition();
    Eigen::Vector3f getDetectorCenter();

    void setCenter(Eigen::Vector3f center);
    void setRotation(float rotation);
    
    Eigen::ParametrizedLine<float, 3> getRay(int horizontal, int vertical);
};

#endif