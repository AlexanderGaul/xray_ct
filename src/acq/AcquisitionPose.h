#ifndef ACQUISITIONPOSE_H
#define ACQUISITIONPOSE_H

#include <array>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <QObject>

class AcquisitionPose
{
private:
    float s2dd_;

    Eigen::Vector3f src_;
    Eigen::Vector3f det_;
    Eigen::Vector3f det_normal_;
    Eigen::Vector3f det_upl_;

    Eigen::Vector3f center_;

    Eigen::AngleAxisf _rot;

    float det_width_;
    float det_height_;

    int pxl_horizontal_;
    int pxl_vertical_;

    void updatePose();
    Eigen::Matrix3f getRot() const;

public:
    AcquisitionPose(const Eigen::AlignedBox<float, 3>& boundingBox)
        : AcquisitionPose(boundingBox.diagonal().norm()*2.f, 0.8f, 0.8f, 50, 50) {
            
        } 

    AcquisitionPose(float s2dd, float det_w, float det_h, int pixel_h, int pixel_v);

    Eigen::Vector3f getSourcePosition() const;
    Eigen::Vector3f getDetectorCenter() const;
    float getDetectorWidth() const;

    Eigen::Vector3f getCenter() const;

    int getPixelHorizontal() const;
    int getPixelVertical() const;

    void setCenter(const Eigen::Vector3f& center);

    void setRotation(Eigen::AngleAxisf rot);
    void setRotation(float rotY, float rotZ);
    void setRotation(float rotZ);
    
    void addRotationZ(float rotZ);
    void addRotationY(float rotY);
    void addRotationX(float rotX);
    
    Eigen::Vector3f getDetectorUpperLeft() const;
    Eigen::Vector3f getDetectorUpperRight() const;
    Eigen::Vector3f getDetectorLowerRight() const;
    Eigen::Vector3f getDetectorLowerLeft() const;
    std::array<Eigen::Vector3f, 4> getDetectorCorners() const;
    
    Eigen::ParametrizedLine<float, 3> getRay(int horizontal, int vertical) const;
    Eigen::Vector3f getPixelCenter(int horizontal, int vertical) const;

    Eigen::ParametrizedLine<float, 3> getRay(int index) const
    {
        return getRay(index / pxl_horizontal_, index % pxl_horizontal_);
    }
};

#endif
