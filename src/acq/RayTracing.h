#ifndef RAYTRACING_H
#define RAYTRACING_H
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <Eigen/Sparse>
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




    /**
     * Computes one row of the system matrix.
     * @brief systemMatrix
     * @param volume
     * @param ray
     * @return
     */
    static
    Eigen::SparseVector<int> systemMatrix(const Volume& volume, const Line3f& ray)
    {
        const auto boundingBox = volume.getBoundingBox();
        const float tIntersect = boxIntersectHelper(boundingBox, ray);

        Eigen::SparseVector<int> ret;

        //The ray doesn't intersect the volume
        if(std::isnan(tIntersect)){
            return ret;
        }

        const Eigen::Vector3f direction {ray.direction()};
        const Eigen::Vector3i step {simpSign(direction.x()), simpSign(direction.y()), simpSign(direction.z())};

        //calculate the maximal absolute boundaries of the box in the direction of the ray
        const float maxX = step.x() <= 0 ? 0 : boundingBox.sizes().x();
        const float maxY = step.y() <= 0 ? 0 : boundingBox.sizes().y();
        const float maxZ = step.z() <= 0 ? 0 : boundingBox.sizes().z();

        const Eigen::Vector3f voxel {volume.getSpacing()};
        const Eigen::Vector3i maxVoxel {volume.getNumVoxels() - Eigen::Vector3i {1, 1, 1}};
        const Eigen::Vector3f tDelta {voxel.cwiseQuotient(direction)};
        const Eigen::Vector3f inPoint {ray.pointAt(tIntersect)};
        Eigen::Vector3i pos { (inPoint - boundingBox.min()).cwiseQuotient(voxel).cast<int>() };

        //calculate fraction

        //calculate the boundaries of a voxel in the direction of the ray
        const float boundX = step.x() <= 0 ? 0 : voxel.x();
        const float boundY = step.y() <= 0 ? 0 : voxel.y();
        const float boundZ = step.z() <= 0 ? 0 : voxel.z();
        const Eigen::Vector3f bounds {boundX, boundY, boundZ};
        //intersection point relative to initially intersected voxel
        const Eigen::Vector3f relativeIntersect {inPoint - pos.cast<float>().cwiseProduct(voxel)};
        //the above in relative distance to the next hit boundary
        const Eigen::Vector3f relativeIntersectFrac {(bounds - relativeIntersect).cwiseQuotient(voxel).cwiseAbs()};

        Eigen::Vector3f tMax {tDelta.cwiseProduct(Eigen::Vector3f {1, 1, 1} - relativeIntersectFrac)};

        if(pos.x() > maxVoxel.x() || pos.y() > maxVoxel.y() || pos.z() > maxVoxel.z())
        {
            return ret;
        }


        for(;;) {
            ret.coeffRef(volume.content().getIndex(pos));

            if(tMax.x() < tMax.y()) {
                if(tMax.x() < tMax.z()) {
                    pos.x() += step.x();
                    if(pos.x() < 0 || pos.x() > maxVoxel.x()) {
                        return ret;
                    }

                    tMax.x() += tDelta.x();
                } else  {
                    pos.z() += step.z();
                    if(pos.z() < 0 || pos.z() > maxVoxel.z()) {
                        return ret;
                    }
                    tMax.z() += tDelta.z();
                }
            } else  {
                if(tMax.y() < tMax.z()) {
                    pos.y() += step.y();
                    if(pos.y() < 0 || pos.y() > maxVoxel.y()){
                        return ret;
                    }
                    tMax.y() += tDelta.y();
                } else  {
                    pos.z() +=  step.z();
                    if(pos.z() < 0 || pos.z() > maxVoxel.z()) {
                        return ret;
                    }
                    tMax.z() += tDelta.z();
                }
            }
        }
    }
    
};

#endif // RAYTRACING_H
