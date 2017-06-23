#include "catch.hpp"

#include <iostream>

#include <Eigen/Eigen>

#include "AcquisitionModel.h"
#include "Vec3D.h"

/**
 * use catch testing for local verification of results.
 * @brief TEST_CASE
 */
TEST_CASE("test sinogram generation")
{
    AcquisitionModel model("/home/chips/dev/cpp_practical/part2-team6/volumes/box.edf");

    // TODO: initialize acquisition pose.
    int SIZE = 10; // we know the size of the test sample
    Eigen::VectorXf content(100);
    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j<10; j++)
        {
            std::cout << model.forwardProject(i,j) << "\t";
            content[i*10+j] = model.forwardProject(i,j);
        }
        std::cout << std::endl;
    }
    Volume volume(Eigen::Vector3f(0,0,0), Eigen::Vector3f(10,10,1), Eigen::Vector3f(1,1,1), content);

    EDFHandler::write("OUTPUT", volume);
}
