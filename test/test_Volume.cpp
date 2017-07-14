#include "catch.hpp"

#include <iostream>

#include <Eigen/Eigen>

#include "Volume.h"

using M=Eigen::VectorXf;

/**
 * use catch testing for local verification of results.
 * @brief TEST_CASE
 */
TEST_CASE("test volume creation")
{
    int n = 2;
    M m(n*n*n);
    for(int i = 0; i<n; ++i)
    {
        for(int j = 0; j<n; ++j)
        {
            for(int k = 0; k<n; ++k)
            {
                m(i*n*n+j*n+k) = (i+j+k)/8.0;
            }
        }
    }

    Eigen::Vector3f lowerLeft(0, 0, 0);
    Eigen::Vector3f upperRight(2, 2, 2);
    Eigen::Vector3f dimSpacing(1, 1, 1);

    Eigen::AlignedBox<float, 3> bb;
    bb.max().z();

    Volume vol(lowerLeft, upperRight, dimSpacing, std::move(m));

    SECTION("trilinear interpolation test")
    {
        // go through one non-int line of the volume
        // and check to known values
        Eigen::VectorXf result(11);
        for(int i = 0; i<11; ++i)
        {
            // after ten steps 3/8 is reached
            // this has to happen in equal intervals
            result(i) = 3.0/80.0*i;
        }

        for(int i = 0; i<=10; ++i)
        {
            Eigen::Vector3f pos(0.1*i, 0.1*i, 0.1*i);
            float f = vol.getVoxelLinear(pos);
            std::cout << f << std::endl;

            REQUIRE(f == Approx(result(i)));
        }
    }
}
