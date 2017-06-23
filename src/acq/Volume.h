#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include <utility>

#include <Eigen/Geometry>

#include "Vec3D.h"

class Volume {
    using content_type = Vec3D<float>;
private:
    Eigen::AlignedBox<float, 3> _boundingBox;
    Eigen::Vector3f _sp;

    content_type _content;
    
    //move the center of the _boundingBox to the origin
    void centerBoundingBox(){
        _boundingBox.translate(-_boundingBox.center());
    }

public:
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp)
        : _boundingBox {lowerLeft, upperRight},
          _sp(sp),
          _content {}
    {
        centerBoundingBox();
    }
    
    template <class Vec>
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp, Vec&& content)
        : _boundingBox {lowerLeft, upperRight},
          _sp(sp),
          _content ((upperRight - lowerLeft).cwiseQuotient(sp).cast<int>(), std::forward<Vec>(content))
    {
        centerBoundingBox();
    }

    Eigen::AlignedBox3f getBoundingBox() const
    {
        return _boundingBox;
    }

    Eigen::Vector3f getSpacing() const
    {
        return _sp;
    }

    Eigen::Vector3i getNumVoxels() const
    {
        return _content.sizes();
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

};

#endif // VOLUME_H
