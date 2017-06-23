#include "catch.hpp"

#include <iostream>

#include <Eigen/Eigen>

#include "AcquisitionModel.h"

/**
 * use catch testing for local verification of results.
 * @brief TEST_CASE
 */
TEST_CASE("test sinogram generation")
{
    AcquisitionModel model("/home/chips/dev/cpp_practical/part2-team6/volumes/box.edf");

    // TODO: initialize acquisition pose.
//    int SIZE = 10; // we know the size of the test sample
//    for(int i = 0; i<10; i++)
//    {
//        for(int j = 0; j<10; j++)
//        {
//            std::cout << model.forwardProject(i,j);
//        }
//        std::cout << std::endl
//    }
}
