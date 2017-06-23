#pragma once

#include <memory>
#include <vector>

#include "AcquisitionPose.h"
#include "RayTracing.h"

/**
 * This class is capable of computing the forward projection.
 *
 * It iterates over all acquisition poses and all rays to compute it.
 * @brief The ForwardProjectionOperator class
 */
class ForwardProjectionOperator
{
private:
    std::unique_ptr<AcquisitionPose>& _pose;
    Volume& _volume;

    std::vector<float> forwardProject(std::size_t row);

    float forwardProject(std::size_t row, std::size_t col);

public:
    ForwardProjectionOperator(Volume& volume, std::unique_ptr<AcquisitionPose>& pose)
        : _volume(volume),
          _pose(pose)
    {

    }

    std::vector<std::vector<float>> forwardProject ();
};
