#pragma once

#include <Eigen/Eigen>

/**
 * This is the abstract container class representing the system matrix B.
 * The kind of B is defined in subclasses.
 * @brief The DataContainer class
 */
class DataContainer
{
public:
    virtual
    void mult(Eigen::Vector3f vector) = 0;
};
