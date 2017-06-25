#include "catch.hpp"

#include <Eigen/Eigen>

#include "CG.h"

/**
 * use catch testing for local verification of results.
 * @brief TEST_CASE
 */
TEST_CASE("test cg computation")
{

    Eigen::VectorXf A(4);
    A << 4, 1, 1, 3;

    Eigen::VectorXf b(2);
    b << 1, 2;

    CG cg;
    //TODO: test

}
