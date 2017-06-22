#pragma once

#include <Eigen/Eigen>

class DataContainer
{
public:
    DataContainer();

    virtual
    void mult(Eigen::Vector3f vector) = 0;
};
