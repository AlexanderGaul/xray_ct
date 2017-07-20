#ifndef POSE_H
#define POSE_H

#include <Eigen/Dense>

/*
 * represents a point that can be rotated around a center with respect to two different axis
 */

class Pose
{
private:
    Eigen::Vector3f _pointRef;
    Eigen::Vector3f _normalRef;

    Eigen::Vector3f _center;
    Eigen::Vector3f _point;
    Eigen::Vector3f _normal;
    
    Eigen::AngleAxisf _rotGlobalZ;
    Eigen::AngleAxisf _rotLocalY;
    Eigen::AngleAxisf _rot;

        
public:
    Pose();
    Pose(Eigen::Vector3f point, Eigen::Vector3f center = Eigen::Vector3f(0, 0, 0));
    
    float getRotationGlobalZ() const;
    float getRotationLocalY() const;
    Eigen::Matrix3f getRotation() const;
    
    void setRotation(float rotGlobalZ, float rotLocalY);
    void setRotationGlobalZ(float globalZ);
    void setRotationLocalY(float localY);
    
    void addRotationGlobalZ(float rotZ);
    void addRotationLocalY(float rotY);
    
    Eigen::Vector3f getCenter() const;
    void setCenter(Eigen::Vector3f center);
    

protected:
    Eigen::Vector3f getPoint() const;
    void setPointRef(Eigen::Vector3f point);
    Eigen::Vector3f getNormal() const;
    
    void setDistanceToCenter(float distance);


    virtual void updatePose();  
};

#endif