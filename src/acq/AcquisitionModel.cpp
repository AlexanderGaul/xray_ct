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

void AcquisitionModel::writeImage(std::string path)
{
    EDFHandler::write(path, _volume);
}

std::vector<float> AcquisitionModel::forwardProject(std::size_t row)
{
    std::vector<float> ret;
    ret.reserve(_pose->getPixelHorizontal());

    for(int x = 0; x < _pose->getPixelHorizontal(); ++x)
    {
        ret.push_back(RayTracing::forwardProject(_volume, _pose->getRay(x, row)));
    }

    return ret;
}

float AcquisitionModel::forwardProject(std::size_t row, std::size_t col)
{
    return RayTracing::forwardProject(_volume, _pose->getRay(col, row));
}


std::vector<std::vector<float>> AcquisitionModel::forwardProject (){
    std::vector<std::vector<float>> ret;
    ret.reserve(_pose->getPixelVertical());
    for(int y = 0; y < _pose->getPixelVertical(); ++y){
        ret.push_back(forwardProject(y));
    }
    return ret;
}
