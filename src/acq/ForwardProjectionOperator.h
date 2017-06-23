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
    //thic class cannot be instantiated
    ForwardProjectionOperator() = delete;
    
    std::vector<float> forwardProject(std::size_t row);

    float forwardProject(std::size_t row, std::size_t col);

public:

    /**
     * Uses only 1 pose.
     * @brief forwardProject
     * @return the forward projection in a ROW MAJOR ORDER
     */
    static std::vector<float> forwardProject(const Volume& vol, const AcquisitionPose& pose);

    /**
     * because of the required return value optimisation there shouldn't be any copies
     * @brief forwardProjectFull
     * @return returns the result of the above function for every pose
     */
    static std::vector<std::vector<float>> forwardProjectFull(const Volume& vol, const std::vector<AcquisitionPose>& poses);
    
    /**
     * TODO
     * Calculates the forwardProjection of steps poses, that are created by rotating pose around the RotationAxis,
     * so that they close a full circle, where "neighboring" poses differ by the same angle
     */
    static std::vector<std::vector<float>> forwardProjectRotateAngle(const Volume&, AcquisitionPose /*,Steps, RotationAxis*/){
        
    };
};
