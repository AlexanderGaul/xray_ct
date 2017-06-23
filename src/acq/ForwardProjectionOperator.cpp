#include "ForwardProjectionOperator.h"


std::vector<float> ForwardProjectionOperator::forwardProject (const Volume& vol, const AcquisitionPose& pose)
{
    std::vector<float> ret;
    ret.reserve(pose.getPixelVertical() * pose.getPixelHorizontal());
    for(int x = 0; x < pose.getPixelHorizontal(); ++x){
        for(int y = 0; y < pose.getPixelVertical(); ++y){
            ret.push_back(RayTracing::forwardProject(vol, pose.getRay(x, y)));
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
