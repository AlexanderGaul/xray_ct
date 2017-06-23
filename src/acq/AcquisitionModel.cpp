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
    _pose {std::make_unique<AcquisitionPose>(_volume.getBoundingBox())},
    _op {std::make_unique<ForwardProjectionOperator>(_volume, _pose) }
{
    QObject::connect(_pose.get(), &AcquisitionPose::poseChanged, this, &AcquisitionModel::poseChanged);
}

void AcquisitionModel::loadImage(std::string path)
{
    _volume = EDFHandler::read(path);
    _pose = std::make_unique<AcquisitionPose>(_volume.getBoundingBox());
    _op = std::make_unique<ForwardProjectionOperator>(_volume, _pose);
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

std::vector<std::vector<float>> AcquisitionModel::forwardProject (){
    return _op->forwardProject();
}
