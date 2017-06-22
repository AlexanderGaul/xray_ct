#pragma once

#include <iostream>

#include <Eigen/Eigen>

#include "DataContainer.h"

class RegularizationContainer : public DataContainer
{
public:
    virtual
    void mult(Eigen::Vector3f vector) override
    {
        std::cout << "TODO: regularized reconstruction" << std::endl;
    }
};
