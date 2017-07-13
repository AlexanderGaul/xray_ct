#include "ForwardProjectionOperator.h"
#include <iostream>


 Eigen::VectorXf ForwardProjectionOperator::forwardProj (const Volume& vol, const AcquisitionPose& pose)
{
    const int verticalPixels = pose.getPixelVertical();
    const int horizontalPixels = pose.getPixelHorizontal();
    Eigen::VectorXf proj {verticalPixels * horizontalPixels};
    //row Major representation
    for(int y = 0; y < pose.getPixelVertical(); ++y){
        int yInd = y * horizontalPixels;
        for(int x = 0; x < pose.getPixelHorizontal(); ++x){
            proj[yInd + x] = std::max(0.f, RayTracing::forwardProject(vol, pose.getRay(x, y), vol.content().rawVec()));
        }
    }
    return proj;
}

Eigen::VectorXf ForwardProjectionOperator::forwardProj(const VolumeBase& vol, const std::vector<AcquisitionPose>& poses, const Eigen::VectorXf& values){
    Eigen::VectorXf proj {(int)poses.size()*poses[0].getPixelHorizontal()*poses[0].getPixelVertical()};
    int poseCount = poses.size();
    int verticalPixels = poses[0].getPixelVertical();
    int horizontalPixels = poses[0].getPixelHorizontal();
    for(int p = 0; p < poseCount;++p){
        for(int y = 0; y < verticalPixels; ++y){
            for(int x = 0; x < horizontalPixels; ++x){
                //TODO some values where negative (but less than 0.01), investigate why this is
                proj[x + y*horizontalPixels + p*horizontalPixels*verticalPixels] = std::max(0.f, RayTracing::forwardProject(vol, poses[p].getRay(x, y), values));
            }
        }
    }
    return proj;
}
