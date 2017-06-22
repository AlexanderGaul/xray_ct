# pragma once

#include <Eigen/Eigen>

#include "DataContainer.h"
#include "Volume.h"

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
     * @param b - solution vector (extracted volume)
     * @param A
     */
    void conjugateGradient(int steps, Volume& b,
                           DataContainer& A)
    {
        Eigen::VectorXf x0;

        //TODO
    }
};
