#include <cmath>

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

std::vector<std::vector<float>> AcquisitionModel::forwardProjectSingle() const{
    auto verticalPixels = currPoseChecked().getPixelVertical();
    auto horizontalPixels = currPoseChecked().getPixelHorizontal();
    std::vector<std::vector<float>> ret;
    ret.reserve(horizontalPixels);
    std::vector<float> temp = ForwardProjectionOperator::forwardProject(_volume, currPoseChecked());
    for(int y = 0; y < verticalPixels; ++ y){
        std::vector<float> row {};
        row.reserve(verticalPixels);
        for(int x = 0; x < horizontalPixels; ++x){
            row.push_back(temp.at(x + y * horizontalPixels));
        }
        ret.push_back(std::move(row));
    }
    return ret;
}

std::vector<std::vector<float>> AcquisitionModel::forwardProjectFull() const{
    return ForwardProjectionOperator::forwardProjectFull(_volume, _poses);
}

Eigen::AlignedBox3f AcquisitionModel::getBoundingBox() const{
        return _volume.getBoundingBox();
    }

void AcquisitionModel::clearPoses() {
        _poses.clear();
        //since the stack must alway contain one element, add the default pose again
        _poses.push_back(AcquisitionPose {getBoundingBox()});
        emit poseChanged();
    }

void AcquisitionModel::deletePose() {
        //since there always has to be an element pop always works
        _poses.pop_back();
        if(_poses.empty()){
            _poses.push_back(AcquisitionPose {getBoundingBox()});
        }
        emit poseChanged();
    }

void AcquisitionModel::savePose() {
        _poses.push_back(AcquisitionPose {getBoundingBox()});
        emit poseChanged();
    }

AcquisitionPose& AcquisitionModel::currPose() {
        return _poses.back();
    }

const AcquisitionPose& AcquisitionModel::currPose() const {
        return _poses.back();
    }

AcquisitionPose& AcquisitionModel::currPoseChecked() {
        if(_poses.empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses.back();
    }

const AcquisitionPose& AcquisitionModel::currPoseChecked() const {
        if(_poses.empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses.back();
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
        AcquisitionPose pose {getBoundingBox()};
        pose.setRotation(zRot, yAngle);
        _poses.push_back(pose);
    }
}
