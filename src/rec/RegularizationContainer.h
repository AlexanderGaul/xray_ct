#pragma once

#include <iostream>

#include <Eigen/Eigen>

#include "DataContainer.h"

class RegularizationContainer : public DataContainer
{
private:
    float _lambda;
public:
    
    RegularizationContainer(const VolumeBase& volume, const std::vector<AcquisitionPose>& poses, float lambda)
        : DataContainer(volume, poses), _lambda {lambda}
    {
    }
    
    virtual Eigen::VectorXf mult(const Eigen::VectorXf vector) const override
    {
        //calculates A^T*A*x
        Eigen::VectorXf res = backwardProj(forwardProj(vector));
        
        //adds lambda*I*x = lambda * x
        res += _lambda*vector;
        
        return res;
    }
};
