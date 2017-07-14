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
    
    Eigen::AngleAxisf _rotGlobalZ;
    Eigen::AngleAxisf _rotLocalY;
    Eigen::AngleAxisf _rot;

    float det_width_;
    float det_height_;

    int pxl_horizontal_;
    int pxl_vertical_;

    void updatePose();
    Eigen::Matrix3f getRot() const;

public:
    AcquisitionPose(const Eigen::AlignedBox<float, 3>& boundingBox)
        : AcquisitionPose(boundingBox.diagonal().norm()*1.5f, 0.8f, 0.8f, 5, 5) {
            
        }

    AcquisitionPose(float s2dd, float det_w, float det_h, int pixel_h, int pixel_v);

    Eigen::Vector3f getSourcePosition() const;
    Eigen::Vector3f getDetectorCenter() const;
    float getDetectorWidth() const;

    Eigen::Vector3f getCenter() const;

    int getPixelHorizontal() const;
    int getPixelVertical() const;
    int getPixelCount() const {
        return getPixelHorizontal()*getPixelVertical();
    }

    void setCenter(const Eigen::Vector3f& center);
    
    float getRotationGlobalZ() const;
    float getRotationLocalY() const;
    
    void setRotation(Eigen::AngleAxisf rot);
    void setRotation(float globalZ, float localY = 0);
    
    void setRotationGlobalZ(float globalZ);
    void setRotationLocalY(float localY);
    
    void addRotationGlobalZ(float rotZ);
    void addRotationLocalY(float rotY);
    
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
