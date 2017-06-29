#pragma once

#include <iostream>
#include <vector>

#include <Eigen/Eigen>

#include "AcquisitionPose.h"
#include "AcquisitionModel.h"
#include "DataContainer.h"
#include "ReconstructionModel.h"
#include "RayTracing.h"

class NormalReconstructionContainer : public DataContainer
{

private:

public:
    NormalReconstructionContainer(const Volume& volume, std::vector<AcquisitionPose>& poses)
        : DataContainer(volume, poses)
    {
    }
    
    virtual Eigen::VectorXf mult(Eigen::VectorXf vector) override
    {
        return backwardProj(forwardProj(vector));
    }
};
