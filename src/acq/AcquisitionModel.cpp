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

void AcquisitionModel::updateRotation(RotationAxis axis, float angle){
    //see description of the rotation axis for why the if is as it is
    if(axis == RotationAxis::Z){
        _pose->setRotationLocalY(_pose->getRotationLocalY()+angle);
    } else {
        _pose->setRotationGlobalZ(_pose->getRotationGlobalZ()+angle);
    }
}

std::array<Eigen::Vector3f, 4> AcquisitionModel::getDetector(){
    return std::array<Eigen::Vector3f, 4> {
        _pose->getDetectorUpperLeft(), _pose->getDetectorLowerLeft(),
        _pose->getDetectorLowerRight(), _pose->getDetectorUpperRight()
    };
}

Eigen::Vector2i AcquisitionModel::getDetectorSize(){
    return Eigen::Vector2i {_pose->getPixelHorizontal(), _pose->getPixelVertical()};
}

Eigen::Vector3f AcquisitionModel::getSourcePosition(){
    return _pose->getSourcePosition();
}

Eigen::Vector3f AcquisitionModel::getPixelCenter(int i, int j){
    return _pose->getPixelCenter(i, j);
}

void AcquisitionModel::writeImage(std::string path)
{
    EDFHandler::write(path, _volume);
}

std::vector<float> AcquisitionModel::forwardProject(){
    return _op->forwardProject();
}

std::vector<std::vector<float>> AcquisitionModel::forwardProjectFull(){
    return _op->forwardProjectFull();
}
