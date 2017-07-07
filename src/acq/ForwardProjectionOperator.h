#pragma once

#include <memory>
#include <vector>

#include <Eigen/Dense>

#include "AcquisitionPose.h"
#include "RayTracing.h"

/**
 * This class is capable of computing the forward projection.
 *
 * It iterates over all acquisition poses and all rays to compute it.
 * @brief The ForwardProjectionOperator class
 */
namespace ForwardProjectionOperator {
    /**
     * Uses only 1 pose.
     * @brief forwardProject
     * @return the forward projection in a ROW MAJOR ORDER
     */
    Eigen::VectorXf forwardProj(const Volume& vol, const AcquisitionPose& pose);
    
    /*
     * Computes A*values, where A is the system matrix of the tomographic problem
     */
    Eigen::VectorXf forwardProj(const VolumeBase& vol, const std::vector<AcquisitionPose>& poses, const Eigen::VectorXf& values);
    
}
