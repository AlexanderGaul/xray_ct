#ifndef RAYTRACING_H
#define RAYTRACING_H
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <limits>
#include <vector>
#include <cmath>

#include "Vec3D.h"
#include "Volume.h"

class RayTracing
{
    using Line3f = Eigen::ParametrizedLine<float, 3>;
    
    //return -1 if the number is negative otherwise +1
    static int simpSign(float x){
        return std::signbit(x) ? -1 : 1;
    }
    
    /*
     * Return a Vector containing only NaNs
     * Represents the the intersections points of a ray and a box if they never actually
     * Intersect
     */
    static constexpr float nanConst = std::numeric_limits<float>::has_signaling_NaN ?
            std::numeric_limits<float>::signaling_NaN() : std::numeric_limits<float>::quiet_NaN();
    
    //return t so that ray.pointAt(t) is the actual intersection point
    static float boxIntersect(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept;
    
    //calculates the forwardProjection of the given ray and the given box
    static float forwardProject(const Volume& box, const Line3f& ray);
    
    static Eigen::Vector3f maxVoxel(Eigen::Vector3f direction){
        
    }
    
};

#endif // RAYTRACING_H
