#ifndef CAMERAPOSE_H
#define CAMERAPOSE_H


/*
 * Acts as the camera setup for the direct volume rendering
 */

class CameraPose : public Pose
{
private:
    
    int _pxlHorizontal;
    int _pxlVertical;
    
    
public:
    
    void setFOV(float fov);
    
    int getPixelHorizontal() const;
    int getPixelVertical() const;
    
    Eigen::ParametrizedLine<float, 3> getRayProjective(int horizontal, int vertical);
    Eigen::ParametrizedLine<float, 3> getRayOrthogonal(int horizontal, int vertical);
};

#endif