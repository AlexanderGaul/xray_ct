#pragma once

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <vector>

#include "AcquisitionPose.h"
#include "RayTracing.h"
#include "ForwardProjectionOperator.h"

/**
 * This is the abstract container class representing the system matrix B.
 * The kind of B is defined in subclasses.
 * @brief The DataContainer class
 */
class DataContainer
{
private:
    VolumeBase _volume;
    std::vector<AcquisitionPose> _poses;
    
public:
    DataContainer(const Volume& volume, std::vector<AcquisitionPose>& poses)
        : _volume {volume}, _poses(poses)
    {
    }

    virtual
    Eigen::VectorXf mult(Eigen::VectorXf vector) = 0;

    VolumeBase getVolume(){
        return _volume;
    }  
    
    /*
     * computes A*x, where A is the system matrix of the tomographic problem (= forward projection)
     */
    Eigen::VectorXf forwardProj(const Eigen::VectorXf& x){
        return ForwardProjectionOperator::forwardProj(_volume, _poses, x);
    }
    
    /*
     * computes A^T*x, where A is the system matrix of the tomographic problem (=backward projection)
     */
    Eigen::VectorXf backwardProj(const Eigen::VectorXf& x){
        return RayTracing::backwardProj(_volume, _poses, x);
    }
};
