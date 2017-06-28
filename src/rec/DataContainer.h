#pragma once

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <vector>

#include "AcquisitionPose.h"

/**
 * This is the abstract container class representing the system matrix B.
 * The kind of B is defined in subclasses.
 * @brief The DataContainer class
 */
class DataContainer
{
private:
    std::vector<AcquisitionPose> poses;
public:
    DataContainer(std::vector<AcquisitionPose>& poses)
        : poses(poses)
    {
    }

    virtual
    Eigen::SparseVector<float> mult(Eigen::VectorXf vector) = 0;

    Eigen::SparseVector<float> row(int index)
    {
        int rowsTotal = poses[0].getPixelVertical();
        int pixelTotal = poses[0].getPixelVertical() * poses[0].getPixelHorizontal();


    }
};
