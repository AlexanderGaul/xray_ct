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
    _poses {_volume.getBoundingBox()}
{
}

void AcquisitionModel::loadImage(std::string path)
{
    _volume = EDFHandler::read(path);
    _poses.push_back(_volume.getBoundingBox());
    if(!checkIfVolumeFitsBlackBox())
    {
        throw std::logic_error("the specified volume does not fit the black box!");
    }
}

void AcquisitionModel::updateRotation(RotationAxis axis, float angle){
    //see description of the rotation axis for why the if is as it is
    if(axis == RotationAxis::Z){
        currPoseChecked().setRotationLocalY(currPoseChecked().getRotationLocalY()+angle);
    } else {
        currPoseChecked().setRotationGlobalZ(currPoseChecked().getRotationGlobalZ()+angle);
    }
}

std::array<Eigen::Vector3f, 4> AcquisitionModel::getDetector() const{
    return std::array<Eigen::Vector3f, 4> {
        currPoseChecked().getDetectorUpperLeft(), currPoseChecked().getDetectorLowerLeft(),
        currPoseChecked().getDetectorLowerRight(), currPoseChecked().getDetectorUpperRight()
    };
}

Eigen::Vector2i AcquisitionModel::getDetectorSize() const{
    return Eigen::Vector2i {currPoseChecked().getPixelHorizontal(), currPoseChecked().getPixelVertical()};
}

Eigen::Vector3f AcquisitionModel::getSourcePosition() const{
    return currPoseChecked().getSourcePosition();
}

Eigen::Vector3f AcquisitionModel::getPixelCenter(int i, int j) const{
    return currPoseChecked().getPixelCenter(i, j);
}

void AcquisitionModel::writeImage(std::string path) const
{
    EDFHandler::write(path, _volume);
}

std::vector<float> AcquisitionModel::forwardProject() const{
    return ForwardProjectionOperator::forwardProject(_volume, currPoseChecked());
}

std::vector<std::vector<float>> AcquisitionModel::forwardProjectFull() const{
    return ForwardProjectionOperator::forwardProjectFull(_volume, _poses);
}

Eigen::AlignedBox3f AcquisitionModel::getBoundingBox() const{
        return _volume.getBoundingBox();
    }
