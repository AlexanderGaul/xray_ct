#ifndef VEC3D_H
#define VEC3D_H

#include <stdexcept>
#include <vector>

#include <Eigen/Dense>

template <class T>
class Vec3D
{
    using index_type = int;
    using vector_type = Eigen::VectorXf;
    using size_vector_type = Eigen::Matrix<index_type, 3, 1>;
    using type = T;
    using typeRef = T&;
private:
    size_vector_type _sizes;
    
    vector_type _content;
        
    void checkBounds(index_type x, index_type y, index_type z) const {
        if(!(x >= 0 && x < sizeX() && y >= 0 && y < sizeY() && z >= 0 && z < sizeZ())){
            std::string exText{"Out of range in 3DImVec; max allowed sizes are: "};
            exText += " x: ";
            exText += std::to_string(sizeX());
            exText += " y: ";
            exText += std::to_string(sizeY());
            exText += " z: ";
            exText += std::to_string(sizeZ());
            
            exText += " but the given indices (equal to size-1) were: ";
            exText += " x: ";
            exText += std::to_string(x);
            exText += " y: ";
            exText += std::to_string(y);
            exText += " z: ";
            exText += std::to_string(z);
        
            throw std::out_of_range {exText}; 
        }
    }
    
    void checkInvariant(size_t vecSize) const{
        if(vecSize != (sizeX() + 1) * (sizeY() + 1) * (sizeZ() + 1)){
            std::string exText {"The vector given as argument of the Vec3D constructor has size "};
            exText += vecSize;
            exText += " but the given size parameters indicate a size of ";
            exText += (sizeX() + 1) * (sizeY() + 1) * (sizeZ() + 1);

            throw std::length_error {exText};
        }
    }
    
public:
    
    Vec3D () : _sizes{size_vector_type::Zero()}, _content {} {    }
    
    Vec3D (index_type maxX, index_type maxY, index_type maxZ, const vector_type& vec) 
    : Vec3D {size_vector_type {maxX, maxY, maxZ}, vec} {
        
    }
    
    Vec3D (index_type maxX, index_type maxY, index_type maxZ, vector_type&& vec) 
    : Vec3D {size_vector_type {maxX, maxY, maxZ}, std::move(vec)} {
        
    }
    
    Vec3D (Eigen::Vector3i maxDims, const vector_type& vec) 
    : _sizes {maxDims}, _content {vec} {
        checkInvariant(vec.size());
    }
    
    Vec3D (Eigen::Vector3i maxDims, vector_type&& vec) 
    : _sizes {maxDims}, _content {} {
        checkInvariant(vec.size());
        //move only when the invariant was already checked
        _content = std::move(vec);
    }
    
    index_type sizeX() const noexcept{
        return _sizes.x();
    }
    
    index_type sizeY() const noexcept{
        return _sizes.y();
    }
    
    index_type sizeZ() const noexcept{
        return _sizes.z();
    }
    
    size_t totalSize() const noexcept{
        return _content.size();
    }
    
    size_vector_type sizes() const noexcept{
        return _sizes;
    }
    
    template <class Vec>
    T& get(Vec vec) noexcept{
        return get(vec.x(), vec.y(), vec.z());
    }
    
    template <class Vec>
    const T& get(Vec vec) const noexcept{
        return get(vec.x(), vec.y(), vec.z());
    }
    
    template <class Vec>
    T& getChecked(Vec vec){
        return getChecked(vec.x(), vec.y(), vec.z());
    }
    
    template <class Vec>
    const T& getChecked(Vec vec) const{
        return getChecked(vec.x(), vec.y(), vec.z());
    }
    
    T& get(int x, int y, int z) noexcept{
        return _content[x + y * sizeX() + z * sizeX() * sizeY()];
    }
    
    const T& get(int x, int y, int z) const noexcept{
        return _content[x + y * sizeX() + z * sizeX() * sizeY()];
    }
    
    T& getChecked(index_type x, index_type y, index_type z){
        checkBounds();
        return get(x, y, z);
    }
    
    const T& getChecked(index_type x, index_type y, index_type z) const {
        checkBounds();
        return get(x, y, z);
    }
    
    T* rawData() noexcept{
        return _content.data(); 
    }
    
    T const * rawData() const noexcept{
        return _content.data(); 
    }
};

#endif // VEC3D_H
