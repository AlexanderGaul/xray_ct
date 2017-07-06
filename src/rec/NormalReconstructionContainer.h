#pragma once

#include <vector>

#include <Eigen/Eigen>

#include "DataContainer.h"

class NormalReconstructionContainer : public DataContainer
{

private:

public:
    NormalReconstructionContainer(const VolumeBase& volume, const std::vector<AcquisitionPose>& poses)
        : DataContainer(volume, poses) { 
            
    }
    
    virtual Eigen::VectorXf mult(Eigen::VectorXf vector) const override
    {
        return backwardProj(forwardProj(vector));
    }
};
