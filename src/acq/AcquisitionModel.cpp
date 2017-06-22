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

AcquisitionModel::AcquisitionModel(std::string path)
    :  _filled {true}, _volume{EDFHandler::read(path)}, 
    _pose {std::make_unique<AcquisitionPose>(_volume.getBoundingBox())}
{
    QObject::connect(_pose.get(), &AcquisitionPose::poseChanged, this, &AcquisitionModel::poseChanged);
}

void AcquisitionModel::loadImage(std::string path)
{
    _volume = EDFHandler::read(path);
    _pose = std::make_unique<AcquisitionPose>(_volume.getBoundingBox());
    QObject::connect(_pose.get(), &AcquisitionPose::poseChanged, this, &AcquisitionModel::poseChanged);
    if(!checkIfVolumeFitsBlackBox())
    {
        throw std::logic_error("the specified volume does not fit the black box!");
    }
}

std::vector< std::vector< float > > AcquisitionModel::forwardProject() {
        std::vector<float> temp {};
        temp.reserve(_pose->getPixelHorizontal());
        
        std::vector<std::vector<float>> ret (_pose->getPixelVertical(), temp);
        for(int y = 0; y < _pose->getPixelVertical(); ++y){
            auto& currRow = ret.at(y);
            for(int x = 0; x < _pose->getPixelHorizontal(); ++x){
                currRow.push_back(RayTracing::forwardProject(_volume, _pose->getRay(x, y)));
            }
        }
        
        return ret;
    }
