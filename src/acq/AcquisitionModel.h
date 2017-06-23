#pragma once
#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>
#include <memory>
#include <array>

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "AcquisitionPose.h"
#include "EDFhandler.h"
#include "ForwardProjectionOperator.h"
#include "RayTracing.h"
#include "Volume.h"

/*
 * Represents an axis of rotation.
 * If something is rotated aroundthe z-axis it is rotated in the x-y-plane
 */
enum class RotationAxis : bool {
    Z,
    X
};

class AcquisitionModel : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief AcquisitionModel. Creates an acquisition model and loads
     * an EDF image from the given path.
     * @param path - file system path determining location of EDF image.
     */
    AcquisitionModel(std::string path);

    /**
     * @brief loadImage. Loads a new EDF image into the acquisition model.
     * @param path - file system path determining location of EDF image.
     */
    void loadImage(std::string path);
    
    /**
     * The next few methods offer an interface to pose that is shown in the the gui.
     * See AcquisitionPose for documentation.
     */
    void updateRotation(RotationAxis axis, float angle);
    
    /**
     * Returns the corners of the detector in the following order:
     * UpperLeft, LowerLeft, LowerRight, UpperRight
     */
    std::array<Eigen::Vector3f, 4> getDetector() const;

    Eigen::Vector2i getDetectorSize() const;

    Eigen::Vector3f getSourcePosition() const;
    
    Eigen::Vector3f getPixelCenter(int i, int j) const;
    
    /*
     * returns the bouding Box of the volume
     */
    Eigen::AlignedBox3f getBoundingBox() const;
  
    /**
     * @brief writeImage. Writes an EDF iamge into a specified path
     * @param path - file system path determining the destination of the image.
     */
    void writeImage(std::string path) const;
    
    /**
     * Calculates the complete forward Projection for the current acquistion pose
     */
    std::vector<float> forwardProject() const;

    /**
     * Calculates the complete forward Projection for all poses stored in the vector
     */
    std::vector<std::vector<float>> forwardProjectFull() const;
    
    /**
     * TODO
     * Calculates a forward Projection for Steps rotations of the current Pose around the
     * Rotation axis
     */
    std::vector<std::vector<float>> forwardProjectAngle(/*Steps, RotationAxis*/){} ;
    
signals:  
    //emited when the acquistion pose changes (because of user action)
    void poseChanged();
private:
    bool checkIfVolumeFitsBlackBox();
    
    /*
     * The pose that is shown on the screen and which can be modified, is the last pose
     * inserted into the poses stack.
     * It is called the current Pose.
     */
    AcquisitionPose& currPose(){
        return _poses.back();
    }
    
    const AcquisitionPose& currPose() const{
        return _poses.back();
    }
    
    AcquisitionPose& currPoseChecked(){
        if(_poses.empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses.back();
    }
    
    const AcquisitionPose& currPoseChecked() const{
        if(_poses.empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses.back();
    }
    
    const Eigen::Vector3f FIXED_BOX_SIZE = Eigen::Vector3f(0.15, 0.15, 0.25);
    bool _filled;
    Volume _volume;
    /*
     * Is effectively a stack. May later be replaced with std::stack TODO
     * Stores older poses that were created by the user, so that a ForwardProjection on 
     * more than one pose can be calculated.
     * There has to be alway one element in this stack, which is the one shown in the gui.
     */
    std::vector<AcquisitionPose> _poses;

};

#endif // ACQUISITIONMODEL_H
