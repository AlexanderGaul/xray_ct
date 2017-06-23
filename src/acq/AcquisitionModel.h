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
    void updateRotation(RotationAxis axis, float angle);
    
    /**
     * Returns the corners of the detector in the following order:
     * UpperLeft, LowerLeft, LowerRight, UpperRight
     */
    std::array<Eigen::Vector3f, 4> getDetector();

    Eigen::Vector2i getDetectorSize();

    Eigen::Vector3f getSourcePosition();
    
    Eigen::Vector3f getPixelCenter(int i, int j);
  
    /**
     * @brief writeImage. Writes an EDF iamge into a specified path
     * @param path - file system path determining the destination of the image.
     */
    void writeImage(std::string path);
    
    /**
     * Calculates the complete forward Projection for the current acquistion pose
     */
    std::vector<float> forwardProject();

    std::vector<std::vector<float>> forwardProjectFull();
signals:  
    //emited when the acquistion pose changes (because of user action)
    void poseChanged();
private:
    
    const Eigen::Vector3f FIXED_BOX_SIZE = Eigen::Vector3f(0.15, 0.15, 0.25);
    bool _filled;
    Volume _volume;
    std::unique_ptr<AcquisitionPose> _pose;
    std::unique_ptr<ForwardProjectionOperator> _op;

};

#endif // ACQUISITIONMODEL_H
