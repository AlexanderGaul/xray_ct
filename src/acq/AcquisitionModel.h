#pragma once
#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>

#include <QFile>
#include <QTextStream>

#include "EDFhandler.h"
#include "Volume.h"
#include "AcquisitionPose.h"
#include "RayTracing.h"

class AcquisitionModel
{
private:
    bool checkIfVolumeFitsBlackBox();
    
public:
    /**
     * @brief AcquisitionModel. Creates an acquisition model with
     * an empty volume.
     */
    AcquisitionModel();

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
private:
    
    const Eigen::Vector3f FIXED_BOX_SIZE = Eigen::Vector3f(0.15, 0.15, 0.25);
    bool _filled;
    Volume _volume;
    AcquisitionPose pose;

};

#endif // ACQUISITIONMODEL_H
