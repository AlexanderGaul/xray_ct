#include "ForwardProjectionOperator.h"


std::vector<float> ForwardProjectionOperator::forwardProject (const Volume& vol, const AcquisitionPose& pose)
{
    std::vector<float> ret;
    ret.reserve(pose.getPixelVertical() * pose.getPixelHorizontal());
    //row Major representation
    for(int y = 0; y < pose.getPixelVertical(); ++y){
        for(int x = 0; x < pose.getPixelHorizontal(); ++x){
            ret.push_back(RayTracing::forwardProject(vol, pose.getRay(x, y), vol.content().rawVec()));
        }
    }
    return ret;
}


std::vector<std::vector<float>> ForwardProjectionOperator::forwardProjectFull(const Volume& vol, const std::vector<AcquisitionPose>& poses)
{
    std::vector<std::vector<float>> ret {};
    ret.reserve(poses.size());
    for(const auto& pose: poses){
        ret.push_back(forwardProject(vol, pose));
    }

    return ret;
}

Eigen::VectorXf forwardProj(const Volume& vol, const std::vector<AcquisitionPose>& poses, const Eigen::VectorXf& values){
    Eigen::VectorXf proj {poses.size()*poses[0].getPixelHorizontal()*poses[0].getPixelVertical()};
    int poseCount = poses.size();
    int verticalPixels = poses[0].getPixelVertical();
    int horizontalPixels = poses[0].getPixelHorizontal();
    for(int p = 0; p < poseCount;++p){
        for(int y = 0; y < verticalPixels; ++y){
            for(int x = 0; x < horizontalPixels; ++x){
                proj[x + y*horizontalPixels + p*horizontalPixels*verticalPixels] = RayTracing::forwardProject(vol, poses[p].getRay(x, y), values);
            }
        }
    }
    return proj;
}
