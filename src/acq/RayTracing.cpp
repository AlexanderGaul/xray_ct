#include "RayTracing.h"

float RayTracing::boxIntersectHelper(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept {
        //based on "An Efficient and Robust Ray–Box Intersection Algorithm"
        //link:  http://people.csail.mit.edu/amy/papers/box-jgt.pdf
        float tmin, tmax, tymin, tymax, tzmin, tzmax;
        const auto boxMin = box.min();
        const auto boxMax = box.max();
        
        {
            const float divx = 1/ray.direction().x(); 
            if(ray.direction().x() >= 0){
                tmin = (boxMin.x() - ray.origin().x()) * divx;
                tmax = (boxMax.x() - ray.origin().x()) * divx;
            } else {
                tmin = (boxMax.x() - ray.origin().x()) * divx;
                tmax = (boxMin.x() - ray.origin().x()) * divx;
            }
        }
        
        {
            const float divy = 1/ray.direction().y(); 
            if(ray.direction().y() >= 0){
                tymin = (boxMin.y() - ray.origin().y()) * divy;
                tymax = (boxMax.y() - ray.origin().y()) * divy;
            } else {
                tymin = (boxMax.y() - ray.origin().y()) * divy;
                tymax = (boxMin.y() - ray.origin().y()) * divy;
            }
        }
        
        if ( (tmin > tymax)|| (tymin > tmax)){
            return nanConst;
        }
        
        if (tymin> tmin){
            tmin = tymin;
        }
        
        if (tymax < tmax){
            tmax = tymax;
        }
        
        {
            const float divz = 1/ray.direction().z(); 
            if(ray.direction().z() >= 0){
                tzmin = (boxMin.z() - ray.origin().z()) * divz;
                tzmax = (boxMax.z() - ray.origin().z()) * divz;
            } else {
                tzmin = (boxMax.z() - ray.origin().z()) * divz;
                tzmax = (boxMin.z() - ray.origin().z()) * divz;
            }
            
        }
        
        if ( (tmin > tzmax)|| (tzmin > tmax)){
            return nanConst;
        }
        
        if (tzmin > tmin) {
            tmin = tzmin;
        }
        
        return tmin;
    }


Eigen::Vector3f RayTracing::boxIntersect(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept
{
    return Eigen::Vector3f(ray.origin() + ray.direction() * boxIntersectHelper(box, ray));
}

float RayTracing::forwardProject(const Volume& volume, const Line3f& ray) {
        const auto boundingBox = volume.getBoundingBox();
        const float tIntersect = boxIntersectHelper(boundingBox, ray);
        
        //The ray doesn't intersect the volume
        if(std::isnan(tIntersect)){
            return 0; //std::vector<Eigen::Vector3f> {}
        }
        
        const Eigen::Vector3f direction {ray.direction()};
        const Eigen::Vector3i step {simpSign(direction.x()), simpSign(direction.y()), simpSign(direction.z())};
        
        //calculate the maximal absolute boundaries of the box in the direction of the ray
        const float maxX = step.x() <= 0 ? 0 : boundingBox.sizes().x();
        const float maxY = step.y() <= 0 ? 0 : boundingBox.sizes().y();
        const float maxZ = step.z() <= 0 ? 0 : boundingBox.sizes().z();
        
        const Eigen::Vector3f voxel {volume.getSpacing()};
        const Eigen::Vector3f maxVoxel {Eigen::Vector3f(maxX, maxY, maxZ).cwiseQuotient(voxel)};
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
        
        float acc = 0;
        
        for(;;) {
            acc += volume.content().get(pos);
            if(tMax.x() < tMax.y()) { 
                if(tMax.x() < tMax.z()) {
                    pos.x() += step.x();
                    if(pos.x() > maxVoxel.x()) {
                        return acc;
                    }
                    
                    tMax.x() += tDelta.x();
                } else  {
                    pos.z() += step.z();
                    if(pos.z() > maxVoxel.z()) {
                        return acc;
                    }
                    tMax.z() += tDelta.z();
                }
            } else  {
                if(tMax.y() < tMax.z()) {
                    pos.y() += step.y();
                    if(pos.y() > maxVoxel.y()){
                        return acc;
                    }
                    tMax.y() += tDelta.y();
                } else  {
                    pos.z() +=  step.z();
                    if(pos.z() > maxVoxel.z()) {
                        return acc;
                    }
                    tMax.z() += tDelta.z();
                }
            }
        }       
    }
