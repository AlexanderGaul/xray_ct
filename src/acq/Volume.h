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
    VolumeBase(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp)
        : _boundingBox {lowerLeft, upperRight},
          _numVoxels {(_boundingBox.max() - _boundingBox.min()).cwiseQuotient(sp).cast<int>()}
    {
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

    float computeMaxEntry()
    {
        float max = 0.0;
        for(int i = 0; i<_content.sizeX(); ++i)
        {
         for(int j = 0; j<_content.sizeY(); ++j)
         {
             for(int k = 0; k<_content.sizeZ(); ++k)
             {
                 if(_content.get(i, j, k) > max)
                 {
                     max = _content.get(i,j,k);
                 }
             }
         }
        }
        return max;
    }
    
public:
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp)
        : Volume {lowerLeft, upperRight, sp, Eigen::VectorXf{}}
    {
        centerBoundingBox();
    }
    
    template <class Vec>
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp, Vec&& content)
        : VolumeBase {lowerLeft, upperRight, sp},
          _content (getNumVoxels(), std::forward<Vec>(content)),
          _maxEntry(computeMaxEntry())
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
    }
    
    void setContent(content_type&& content)
    {
        _content = std::move(content);
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

};

#endif // VOLUME_H
