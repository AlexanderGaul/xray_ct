#pragma once

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <vector>

#include "AcquisitionPose.h"
#include "RayTracing.h"
#include "ForwardProjectionOperator.h"
#include "Volume.h"


/**
 * This is the abstract container class representing the system matrix B.
 * The kind of B is defined in subclasses.
 * @brief The DataContainer class
 */
class DataContainer{
protected:
    const VolumeBase& _base;
    const std::vector<AcquisitionPose>& _poses;
    
public:
    
    DataContainer(const VolumeBase& volumeBase, const std::vector<AcquisitionPose>& poses) 
        : _base {volumeBase}, _poses {poses} {
            
    }

    virtual Eigen::VectorXf mult(Eigen::VectorXf vector) const= 0;

    /*
     * computes A*x, where A is the system matrix of the tomographic problem (= forward projection)
     */
    Eigen::VectorXf forwardProj( const Eigen::VectorXf& x) const{
        return ForwardProjectionOperator::forwardProj(_base, _poses, x);
    }
    
    /*
     * computes A^T*x, where A is the system matrix of the tomographic problem (=backward projection)
     */
    Eigen::VectorXf backwardProj(const Eigen::VectorXf& x) const {
        return RayTracing::backwardProj(_base, _poses, x);
    }
    
    int getTotalVoxelCount() const {
        return _base.getTotalVoxelCount();
    }
    
    const VolumeBase& getVolume(){
        return _base;
    }
};
