#ifndef CAMERAPOSE_H
#define CAMERAPOSE_H

#include <Eigen/Dense>

#include "Pose.h"

/*
 * Acts as the camera setup for the direct volume rendering
 */

class CameraPose : public Pose
{
private:
    
    int _pxlHorizontal;
    int _pxlVertical;
    
    float _fov;
    float _pixelSize;
    
    Eigen::Vector3f _down;
    Eigen::Vector3f _right;
    Eigen::Vector3f _screenCenter;
public:
    CameraPose();
    CameraPose(int pixelHorizontal, int pixelVertical, float distance = 0.5f);
    
    void setFov(float fov);
    
    int getPixelHorizontal() const;
    int getPixelVertical() const;
    
    Eigen::ParametrizedLine<float, 3> getRayProjective(int horizontal, int vertical);
    Eigen::ParametrizedLine<float, 3> getRayOrthogonal(int horizontal, int vertical);
    
    void setDistance(float distance);
    float getDistance();
    
protected:
    virtual void updatePose() override;
    
public:
    Eigen::Vector3f getPixel(int horizontal, int vertical);
private:
    Eigen::Vector3f getPixel(int horizontal, int vertical, Eigen::Vector3f screenCenter);
};

#endif