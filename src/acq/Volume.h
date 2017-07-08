#ifndef VOLUME_H
#define VOLUME_H

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
    
    float getVoxel(Eigen::Vector3i pos)
    {
        return _content.rawVec()[coordinateToIndex(pos)];
    }
    float getVoxel(int x, int y, int z)
    {
        return getVoxel(Eigen::Vector3i(x, y, z));
    }
    /*
     * trillinear interpolation with surrounding voxels
     * returns zero if outside volume
     * extrapolates trilinearly if outside voxel centers
     * 
     * THIS CODE HAS NEITHER BEEN TESTED NOR REVIEWED
     * HANDLE WITH CARE
     */
    float getVoxelLinear(Eigen::Vector3f position)
    {
        Eigen::Vector3f relPos = position - _boundingBox.min();
        
        if(position.x() > _boundingBox.max().x()
        || position.y() > _boundingBox.max().y()
        || position.z() > _boundingBox.max().z())
        {
            return 0;
        }
        if(position.x() < 0
        || position.y() < 0
        || position.z() < 0)
        {
            return 0;
        }
        
        Eigen::Vector3f spacing = getSpacing();
        Eigen::Vector3f halfVoxel = spacing * 0.5f;
        
        
        Eigen::Vector3f voxelPos = (relPos - halfVoxel).cwiseQuotient(spacing);
        
        // index of voxel centers surrounding the position
        Eigen::Vector3i minVoxel = voxelPos.cast<int>();
        Eigen::Vector3i maxVoxel = minVoxel.cast<int>() + Eigen::Vector3i(1, 1, 1);
        
        // in case of rounding errors?
        for(int i = 0; i < 3; i++)
        {
            if(minVoxel(i) < 0)
            { minVoxel(i) = 0; }
            if(maxVoxel(i) > getNumVoxels()(i) - 1)
            { maxVoxel(i) = getNumVoxels()(i) - 1; }
        }
        
        // relative position between surrounding voxel centers
        voxelPos = voxelPos - minVoxel.cast<float>();
        
        Eigen::Vector4f firstValues {
            getVoxel(minVoxel),
            getVoxel(minVoxel + Eigen::Vector3i(0, 1, 0)),
            getVoxel(minVoxel + Eigen::Vector3i(0, 0, 1)),
            getVoxel(minVoxel + Eigen::Vector3i(0, 1, 1))
        };
        Eigen::Vector4f secondValues {
            getVoxel(maxVoxel - Eigen::Vector3i(0, 1, 1)),
            getVoxel(maxVoxel - Eigen::Vector3i(0, 0, 1)),
            getVoxel(maxVoxel - Eigen::Vector3i(0, 1, 0)),
            getVoxel(maxVoxel)
        };
        
        Eigen::Vector4f interpX = firstValues + voxelPos.x() * (secondValues - firstValues);
        
        Eigen::Vector2f interpXY {
            interpX(0) + voxelPos.y() * (interpX(1) - interpX(0)),
            interpX(2) + voxelPos.y() * (interpX(3) - interpX(2))
        };
        
        float interpXYZ = interpXY(0) + voxelPos.z() * (interpXY(1) - interpXY(0));

        return interpXYZ;
    }
    float getVoxelLinear(float x, float y, float z)
    {
        return getVoxelLinear(Eigen::Vector3f(x, y, z));
    }

};

#endif // VOLUME_H
