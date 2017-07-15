#ifndef VOLUME_H
#define VOLUME_H

#include <cmath>
#include <string>
#include <utility>

#include <Eigen/Geometry>

#include "Vec3D.h"

class VolumeBase{    
protected:
    Eigen::AlignedBox<float, 3> _boundingBox;
    Eigen::Vector3i _numVoxels;
    
public:
    VolumeBase()
        : VolumeBase {Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero(), Eigen::Vector3f {1, 1, 1}}{
            
    }
    
    VolumeBase(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp)
        : _boundingBox {lowerLeft, upperRight},
          _numVoxels {(_boundingBox.max() - _boundingBox.min()).cwiseQuotient(sp).cast<int>()} {
    }
    
    
    Eigen::AlignedBox3f getBoundingBox() const
    {
        return _boundingBox;
    }

    Eigen::Vector3f getSpacing() const
    {
        return (_boundingBox.max() - _boundingBox.min()).cwiseQuotient(_numVoxels.cast<float>());
    }

    Eigen::Vector3i getNumVoxels() const
    {
        return _numVoxels;
    }
    
    int getTotalVoxelCount() const {
        auto numVoxels = getNumVoxels();
        return numVoxels.x() * numVoxels.y() * numVoxels.z();
    }
    
    int coordinateToIndex(Eigen::Vector3i coor) const {
        auto numVoxels = getNumVoxels();
        return coor.x() + coor.y() * numVoxels.x() + coor.z() * numVoxels.x() * numVoxels.y(); 
    }
    
    Eigen::Vector3i indexToCoordinate(int index){
        auto numVoxels = getNumVoxels();
        //round as late as possible
        const int zIndex = index/(numVoxels.x()*numVoxels.y());
        const int temp = index%(numVoxels.x()*numVoxels.y());
        const int yIndex = temp/(numVoxels.x());
        const int xIndex = temp%(numVoxels.x());
        
        return Eigen::Vector3i {xIndex, yIndex, zIndex};
    }
};

class Volume : public VolumeBase{
private:
    using content_type = Vec3D<float>;

    content_type _content;

    float _maxEntry;

    //move the center of the _boundingBox to the origin
    void centerBoundingBox(){
        _boundingBox.translate(-_boundingBox.center());
    }

    bool validPosition(Eigen::Vector3f& position) const
    {
        if(position.x() > getNumVoxels()[0]
            || position.y() > getNumVoxels()[1]
            || position.z() > getNumVoxels()[2]
            || position.x() < 0
            || position.y() < 0
            || position.z() < 0)
        {
            return false;
        }
        return true;
    }
    
public:
    Volume() : VolumeBase {}, _content {}{
        
    }
    
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp)
        : Volume {lowerLeft, upperRight, sp, Eigen::VectorXf{}}
    {
        centerBoundingBox();
    }
    
    template <class Vec>
    Volume(const VolumeBase& volume, Vec&& content)
        : VolumeBase {volume}, _content {getNumVoxels(), std::forward<Vec>(content)},
          _maxEntry {_content.maxEntry()}
    {
        centerBoundingBox();
    }
    
    template <class Vec>
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp, Vec&& content)
        : VolumeBase {lowerLeft, upperRight, sp},
          _content (getNumVoxels(), std::forward<Vec>(content)),
          _maxEntry(_content.maxEntry())
    {
        centerBoundingBox();
    }
    
    const content_type& content() const
    {
        return _content;
    }

    void setContent(content_type& content)
    {
        _content = content;
        _maxEntry = _content.maxEntry();
    }
    
    void setContent(content_type&& content)
    {
        _content = std::move(content);
        _maxEntry = _content.maxEntry();
    }

    void computeMaxEntry()
    {
        _maxEntry = _content.maxEntry();
    }

    /**
     * Return the maximum level of brightness.
     * (this is computed via computeMaxEntry(), called from the
     * constructors).
     * This is used to show each image with good
     * contrast on the screen.
     * @brief maxEntry
     * @return
     */
    float maxEntry() const
    {
        return _maxEntry;
    }
    
    float getVoxel(Eigen::Vector3i pos) const
    {
        return _content.rawVec()(coordinateToIndex(pos));
    }

    float getVoxel(int x, int y, int z) const
    {
        return getVoxel(Eigen::Vector3i(x, y, z));
    }

    /**
     * Trillinear interpolation with surrounding voxels
     * returns zero if outside volume
     * extrapolates trilinearly if outside voxel centers
     *
     * Tested in test_Volume.cpp.
     *
     * Handle special case of integer index:
     * Go down to the next lower voxel and interpolate this
     * voxel. No additional computational effort
     * or handling required.
     *
     * @brief getVoxelLinear
     * @param position
     * @return
     */
    float getVoxelLinear(Eigen::Vector3f position) const
    {
        if(!validPosition(position))
        {
            return -1.0;
        }

        // index of voxel centers surrounding the position
        Eigen::Vector3i minVoxel = position.cast<int>();
        for(int i = 0; i<3; ++i)
        {
            if(minVoxel[i] > 0 && minVoxel[i] - position[i] >= -0.0001)
            {
                minVoxel[i]--;
            }
        }

        // relative position between surrounding voxel centers
        Eigen::Vector3f voxelPos(3);
        for(int i = 0; i<3; ++i)
        {
            if(minVoxel(i) == 0)
            {
                voxelPos(i) = position(i);
            }
            else
            {
                voxelPos(i) = fmod(position(i),minVoxel(i));
            }
        }

        Eigen::Vector4f firstValues {
            getVoxel(minVoxel),
            getVoxel(minVoxel + Eigen::Vector3i(0, 1, 0)),
            getVoxel(minVoxel + Eigen::Vector3i(0, 0, 1)),
            getVoxel(minVoxel + Eigen::Vector3i(0, 1, 1))
        };
        Eigen::Vector4f secondValues {
            getVoxel(minVoxel + Eigen::Vector3i(1, 0, 0)),
            getVoxel(minVoxel + Eigen::Vector3i(1, 1, 0)),
            getVoxel(minVoxel + Eigen::Vector3i(1, 0, 1)),
            getVoxel(minVoxel + Eigen::Vector3i(1,1,1))
        };
        
        Eigen::Vector4f interpX = firstValues + voxelPos.x() * (secondValues - firstValues);
        
        Eigen::Vector2f interpXY {
            interpX(0) + voxelPos.y() * (interpX(1) - interpX(0)),
            interpX(2) + voxelPos.y() * (interpX(3) - interpX(2))
        };
        
        float interpXYZ = interpXY(0) + voxelPos.z() * (interpXY(1) - interpXY(0));

        return interpXYZ;
    }

    /**
     * Performs trilinear interpolation for arbitrary
     * (maybe non-integer) indices x, y, and z.
     *
     * Internally creates a Eigen::Vector to call the real
     * implementation of trilinear interpolation.
     * @brief getVoxelLinear
     * @param x the x coordinate to be interpolated
     * @param y the y coordinate to be interpolated
     * @param z the z coordinate to be interpolated
     * @return The value at the (non-integer) coordinate specified
     * by the three input floats. It is computed using trilinear
     * interpolation between the surrounding voxels.
     */
    float getVoxelLinear(float x, float y, float z) const
    {
        return getVoxelLinear(Eigen::Vector3f(x, y, z));
    }

};

#endif // VOLUME_H
