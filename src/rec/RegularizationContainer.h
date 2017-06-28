#pragma once

#include <iostream>

#include <Eigen/Eigen>

#include "DataContainer.h"

class RegularizationContainer : public DataContainer
{
public:
    virtual
    Eigen::SparseVector<float> mult(Eigen::VectorXf vector) override
    {
        std::cout << "TODO: regularized reconstruction" << std::endl;
    }
};
