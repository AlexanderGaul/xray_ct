#ifndef RAYTRACING_H
#define RAYTRACING_H
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "Vec3D.h"
#include "Volume.h"

class RayTracing
{
    using Line3f = Eigen::ParametrizedLine<float, 3>;
    
    //return -1 if the number is negative otherwise +1
    static int simpSign(float x){
        return std::signbit(x) ? -1 : 1;
    }
    

    /**
     * return t so that ray.pointAt(t) is the actual intersection point
     */
    static float boxIntersectHelper(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept;

    /*
     * Used as a return value
     * Represents the the intersections points of a ray and a box if they never actually
     * Intersect.
     * Should never be used but caught before it can be used in a calculation
     * If available this is a signaling NaN which might make the usage of this in a 
     * calculation easier to spot
     */
    static constexpr float nanConst = std::numeric_limits<float>::has_signaling_NaN ?
            std::numeric_limits<float>::signaling_NaN() : std::numeric_limits<float>::quiet_NaN();
    
public:

    /**
     *
     * @brief boxIntersect Returns the intersection point of the ray with the box.
     * @return an 3x1 Eigen Vector which contains the point of intersection.
     */
    static Eigen::Vector3f boxIntersect(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept;
    
    /**
     * calculates the forwardProjection of the given ray and the given box.
     * @return sum of all contributions of the ray going through the box
     */
    static float forwardProject(const Volume& box, const Line3f& ray);
    
};

#endif // RAYTRACING_H
