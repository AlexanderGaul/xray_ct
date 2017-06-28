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
    NormalReconstructionContainer(std::vector<AcquisitionPose>& poses)
        : DataContainer(poses)
    {
    }

    Eigen::SparseVector<int> systemMatrix(int row)
    {
        return RayTracing::systemMatrix(_volume, _rays);
    }

    virtual
    Eigen::SparseVector<float> mult(Eigen::VectorXf vector) override
    {
        Eigen::VectorXf res(vector.rows());
        for(int i = 0; i<vector.rows(); ++i)
        {
            res(i) = getRow(i) * vector;
        }
        return res;
    }
};
