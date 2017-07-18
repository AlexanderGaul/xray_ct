#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <memory>

#include <Eigen/Dense>

#include "Volume.h"
#include "AcquisitionPose.h"

/*
 * Since the information needed for the Reconstruction is quite complex, it is wrapped
 * in a struct when it goes from the AcquistionModel to the ReconstructionModel.
 * 
 * In the model it isn't used right now. It is only constructed for the transport and than
 * destructed again
 */
struct Acquisition
{
    VolumeBase volBase;
    std::shared_ptr<const std::vector<AcquisitionPose>> poses;
    std::shared_ptr<const Eigen::VectorXf> measurements;
    
    Acquisition(const VolumeBase& volBase, std::shared_ptr<const std::vector<AcquisitionPose>> poses, 
        std::shared_ptr<const Eigen::VectorXf> measurements
    ) : volBase {volBase}, poses {poses}, measurements {measurements} {
        
    }
};

#endif // ACQUISITION_H
