#pragma once
#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>
#include <memory>
#include <array>

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "EDFhandler.h"
#include "Volume.h"
#include "AcquisitionPose.h"
#include "RayTracing.h"

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
private:
    bool checkIfVolumeFitsBlackBox();
    
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
     * The next few methods offer an interface to the pose for the gui.
     * See AcquisitionPose for documentation.
     */
    void updateRotation(RotationAxis axis, float angle){
        //see description of the rotation axis for why the if is as it is
        if(axis == RotationAxis::Z){
            _pose->setRotationLocalY(_pose->getRotationLocalY()+angle);
        } else {
            _pose->setRotationGlobalZ(_pose->getRotationGlobalZ()+angle);
        }
    }
    
    /*
     * Returns the corners of the detector in the following order:
     * UpperLeft, LowerLeft, LowerRight, UpperRight
     */
    std::array<Eigen::Vector3f, 4> getDetector(){
        return std::array<Eigen::Vector3f, 4> {
            _pose->getDetectorUpperLeft(), _pose->getDetectorLowerLeft(),
            _pose->getDetectorLowerRight(), _pose->getDetectorUpperRight()
        };
    }
    
    Eigen::Vector2i getDetectorSize(){
        return Eigen::Vector2i {_pose->getPixelHorizontal(), _pose->getPixelVertical()};
    }
    
    Eigen::Vector3f getSourcePosition(){
        return _pose->getSourcePosition();
    }
    
    Eigen::Vector3f getPixelCenter(int i, int j){
        return _pose->getPixelCenter(i, j);
    }
  
    /**
     * @brief writeImage. Writes an EDF iamge into a specified path
     * @param path - file system path determining the destination of the image.
     */
    void writeImage(std::string path);

    /**
     * @brief forwardProject Computes the forward projection on-the-fly (only calculates one row of system matrix).
     * @param row - specifies the row of the system matrix to be computed
     * @return one row of the system matrix
     */
    std::vector<float> forwardProject(std::size_t row);

    float forwardProject(std::size_t row, std::size_t col);
    
    /*
     * Calculates the complete forward Projection for the current acquistion pose
     */
    std::vector<std::vector<float>> forwardProject ();
signals:  
    //emited when the acquistion pose changes (because of user action)
    void poseChanged();
private:
    
    const Eigen::Vector3f FIXED_BOX_SIZE = Eigen::Vector3f(0.15, 0.15, 0.25);
    bool _filled;
    Volume _volume;
    std::unique_ptr<AcquisitionPose> _pose;

};

#endif // ACQUISITIONMODEL_H
