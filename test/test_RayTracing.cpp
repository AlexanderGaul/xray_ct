#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <Eigen/Eigen>

#include "RayTracing.h"

/**
 * use catch testing for local verification of results.
 * @brief TEST_CASE
 */
TEST_CASE("test ray tracing")
{
    // create the aligned box
    Eigen::Vector3f min(0,0,0);
    Eigen::Vector3f max(1,1,0);
    Eigen::AlignedBox3f box(min, max);
    SECTION("orthogonal line")
    {
        // create the ray
        Eigen::Vector3f origin(0.5, 0.5, -1);
        Eigen::Vector3f direction(0, 0, 1);
        Eigen::ParametrizedLine<float, 3> ray(origin, direction);
        float scaling = RayTracing::boxIntersect(box, ray);

        REQUIRE(scaling == Approx(1.0));
    }
    SECTION("askew line")
    {
        // create the ray
        Eigen::Vector3f origin(1, 1, -1);
        Eigen::Vector3f direction(-0.5, -0.5, 1);
        Eigen::ParametrizedLine<float, 3> ray(origin, direction);
        float scaling = RayTracing::boxIntersect(box, ray);

        REQUIRE(scaling == Approx(1.0));

        Eigen::Vector3f intersection(origin + direction * scaling);

        REQUIRE(intersection[0] == 0.5);
        REQUIRE(intersection[1] == 0.5);
        REQUIRE(intersection[2] == 0);
    }
}
