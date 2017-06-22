#pragma once

#include <iostream>

#include "DataContainer.h"

class NormalReconstructionContainer : public DataContainer
{

    virtual
    void mult(Eigen::Vector3f vector) override
    {
        std::cout << "TODO: normal reconstruction" << std::endl;
    }
};
