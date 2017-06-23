#ifndef ACQUISITIONPOSE_H
#define ACQUISITIONPOSE_H

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

    Eigen::AngleAxisf rot_global_z_;
    Eigen::AngleAxisf rot_local_y_;

    float det_width_;
    float det_height_;

    int pxl_horizontal_;
    int pxl_vertical_;

    void updatePose();
    Eigen::Matrix3f getRot() const;

public:
    AcquisitionPose(const Eigen::AlignedBox<float, 3>& boundingBox)
        : AcquisitionPose(boundingBox.diagonal().norm()*1.5, 0.4, 0.4, 5, 5) {
            
        } 

    AcquisitionPose(float s2dd, float det_w, float det_h, int pixel_h, int pixel_v);

    Eigen::Vector3f getSourcePosition() const;
    Eigen::Vector3f getDetectorCenter() const;
    float getDetectorWidth() const;

    Eigen::Vector3f getCenter() const;
    
    //float getRotation();
    float getRotationGlobalZ() const;
    float getRotationLocalY() const;

    int getPixelHorizontal() const;
    int getPixelVertical() const;

    void setCenter(const Eigen::Vector3f& center);

    void setRotation(float rot_global_z, float rot_local_y_ = 0);
    void setRotationGlobalZ(float rot_global_z);
    void setRotationLocalY(float rot_local_y);
    
    Eigen::Vector3f getDetectorUpperLeft() const;
    Eigen::Vector3f getDetectorUpperRight() const;
    Eigen::Vector3f getDetectorLowerRight() const;
    Eigen::Vector3f getDetectorLowerLeft() const;
    
    Eigen::ParametrizedLine<float, 3> getRay(int horizontal, int vertical) const;
    Eigen::Vector3f getPixelCenter(int horizontal, int vertical) const;

};

#endif
