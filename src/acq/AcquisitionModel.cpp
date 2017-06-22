#include "AcquisitionModel.h"

bool AcquisitionModel::checkIfVolumeFitsBlackBox()
{
    // check if the volume fits the black box
    Eigen::Vector3i voxels = _volume.getNumVoxels();
    Eigen::Vector3f spacing = _volume.getSpacing();

    for(int i = 0; i<3; i++)
    {
        if(voxels[i] * spacing[i] > FIXED_BOX_SIZE[i])
        {
            return false;
        }
    }
    return true;
}

AcquisitionModel::AcquisitionModel()
    : _volume(Eigen::Vector3f(-1,-1,-1),
              Eigen::Vector3f(-1,-1,-1),
              Eigen::Vector3f(-1,-1,-1)
              )
{
    _filled = false;
}


AcquisitionModel::AcquisitionModel(std::string path)
    : _volume(EDFHandler::read(path))
{
    _filled = true;
}

void AcquisitionModel::loadImage(std::string path)
{
    _volume = EDFHandler::read(path);
    if(!checkIfVolumeFitsBlackBox())
    {
        throw std::logic_error("the specified volume does not fit the black box!");
    }
}

std::vector< std::vector< float > > AcquisitionModel::forwardProject() {
        std::vector<float> temp {};
        temp.reserve(pose.getPixelHorizontal());
        
        std::vector<std::vector<float>> ret (pose.getPixelVertical(), temp);
        for(int y = 0; y < pose.getPixelVertical(); ++y){
            auto& currRow = ret[y];
            for(int x = 0; x < pose.getPixelHorizontal(); ++x){
                currRow[x] = RayTracing::forwardProject(_volume, pose.getRay(x, y));
            }
        }
        
        return ret;
    }
