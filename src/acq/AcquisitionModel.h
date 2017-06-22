#pragma once
#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>
#include <memory>

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
     * @brief forwardProject. calculates the forward projection of the current acquistion pose.
     * @return returns the forward Projection
     */
    std::vector<std::vector<float>> forwardProject();
    
    /**
     * The next few methods offer an interface to the pose for the gui.
     * See AcquisitionPose for documentation.
     */
    void updateRotation(RotationAxis axis, float angle){
        if(axis == RotationAxis::Z){
            _pose->setRotationLocalY(_pose->getRotationLocalY()+angle);
        } else {
            _pose->setRotationGlobalZ(_pose->getRotationGlobalZ()+angle);
        }
    }
    
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
