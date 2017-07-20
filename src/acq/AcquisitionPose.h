#ifndef ACQUISITIONPOSE_H
#define ACQUISITIONPOSE_H

#include <array>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <QObject>

#include "Pose.h"

class AcquisitionPose : public Pose
{
private:
    float _s2dd;

    Eigen::Vector3f _det;
    Eigen::Vector3f _detUpl;

    float _detWidth;
    float _detHeight;

    int _pxlHorizontal;
    int _pxlVertical;


public:
    
    AcquisitionPose(const Eigen::AlignedBox<float, 3>& boundingBox)
        : AcquisitionPose(boundingBox.diagonal().norm()*1.5f, 0.8f, 0.8f, 20, 20) {
            
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
    
    
    Eigen::Vector3f getDetectorUpperLeft() const;
    Eigen::Vector3f getDetectorUpperRight() const;
    Eigen::Vector3f getDetectorLowerRight() const;
    Eigen::Vector3f getDetectorLowerLeft() const;
    std::array<Eigen::Vector3f, 4> getDetectorCorners() const;
    
    Eigen::ParametrizedLine<float, 3> getRay(int horizontal, int vertical) const;
    Eigen::Vector3f getPixelCenter(int horizontal, int vertical) const;

    Eigen::ParametrizedLine<float, 3> getRay(int index) const
    {
        return getRay(index / _pxlHorizontal, index % _pxlHorizontal);
    }
    
protected:
    void updatePose();
};

#endif
