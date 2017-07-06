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
     * @param b - solution vector (forward Projection)
     * @param A
     */
    static Eigen::VectorXf conjugateGradient(int steps, DataContainer& cont, Eigen::VectorXf b)
    {
        //we need A^T*b and not b
        b = cont.backwardProj(b);
        
        /*
         * This is simply a copy of the algorithm mentioned on the slides
         */
        //init the vector to zero
        Eigen::VectorXf result = Eigen::VectorXf::Zero(cont.getTotalVoxelCount());
        Eigen::VectorXf test = cont.mult(result);
        Eigen::VectorXf r = b - test;
        Eigen::VectorXf d = r;
        float rtr = r.dot(r);
        
        for(int i = 0; i < steps; ++i){
            const Eigen::VectorXf q = cont.mult(d);
            const float alpha = rtr/d.dot(q);
            result += alpha * d;
            r -= alpha * q;
            
            const float oldRtr = rtr;
            rtr = r.dot(r);
            d = r + (rtr/oldRtr * d);
        }
        
        return result;
    }
};
