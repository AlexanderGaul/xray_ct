#include <cmath>

#include "AcquisitionModel.h"

bool AcquisitionModel::checkIfVolumeFitsBlackBox() const
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
    _poses { std::make_shared<std::vector<AcquisitionPose>>( std::vector<AcquisitionPose> {_volume.getBoundingBox()})}, 
    _measurements {std::make_shared<Eigen::VectorXf>(ForwardProjectionOperator::forwardProj(volume(), currPoseChecked()))}
{
}

void AcquisitionModel::loadImage(std::string path)
{
    _volume = EDFHandler::read(path);
    _poses->push_back(_volume.getBoundingBox());
    if(!checkIfVolumeFitsBlackBox())
    {
        throw std::logic_error("the specified volume does not fit the black box!");
    }
}

void AcquisitionModel::updateRotation(RotationAxis axis, float angle){
    //see description of the rotation axis for why the if is as it is
    if(axis == RotationAxis::Z){
        currPoseChecked().addRotationZ(angle);
    } else if (axis == RotationAxis::X) {
        currPoseChecked().addRotationX(angle);
    }
    updateLastProjection();
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

Eigen::AlignedBox3f AcquisitionModel::getBoundingBox() const{
        return _volume.getBoundingBox();
    }

void AcquisitionModel::clearPoses() {
        _poses->clear();
        //since the stack must alway contain one element, add the default pose again
        addDefaultPose();
        updateProjection();
        emit poseChanged();
    }

void AcquisitionModel::deletePose() {
        //since there always has to be an element pop always works
        _poses->pop_back();
        if(_poses->empty()){
            addDefaultPose();
        }
        updateProjection();
        emit poseChanged();
    }

void AcquisitionModel::savePose() {
        addDefaultPose();
        updateProjection();
        emit poseChanged();
    }

AcquisitionPose& AcquisitionModel::currPose() {
        return _poses->back();
    }

const AcquisitionPose& AcquisitionModel::currPose() const {
        return _poses->back();
    }

AcquisitionPose& AcquisitionModel::currPoseChecked() {
        if(_poses->empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses->back();
    }

const AcquisitionPose& AcquisitionModel::currPoseChecked() const {
        if(_poses->empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses->back();
    }


void AcquisitionModel::addSphericalPoses()
{
    
    int count = 5;
    
    float distance = M_PI / count;
    
    for(float yRot = - M_PI / 2.f + distance / 2.f; yRot < M_PI / 2.f; yRot += distance)
    {
        addCircularPoses(yRot);
    }
}

void AcquisitionModel::addHalfSphericalPoses()
{
    int count = 5;
    
    float distance = M_PI / count;
    
    for(float yRot = - M_PI / 2.f + distance / 2.f; yRot < M_PI / 2.f; yRot += distance)
    {
        addCircularPoses(yRot, M_PI);
    }
}   


void AcquisitionModel::addCircularPoses(float yAngle, float range)
{
    
    int count = 10;
    float distance;
    float cosine = cosf(yAngle);
    if(1.f / count > cosine)
    {
        distance = 3 * M_PI;
    }
    else
    {
        distance = range / cosine / count;
    }
    for(float zRot = 0; zRot < range; zRot += distance)
    {
        std::cout << zRot << ", " << yAngle << std::endl;
        AcquisitionPose pose {getBoundingBox()};
        pose.setRotation(yAngle, zRot);
        _poses->push_back(pose);
    }
}
