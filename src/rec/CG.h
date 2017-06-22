# pragma once

#include <Eigen/Eigen>

#include "DataContainer.h"

/**
 * This class implements the conjugate gradient algorithm.
 * @brief The CG class
 */
class CG
{

public:
    /**
     * Computes the conjugate gradient algorithm.
     * The initial guess x_0 is set to the zero vector.
     * @brief conjugateGradient
     * @param steps
     * @param b
     * @param A
     */
    void conjugateGradient(int steps, Eigen::Vector3f b,
                           DataContainer& A)
    {
        Eigen::Vector3f x0(0,0,0);

    }
};
