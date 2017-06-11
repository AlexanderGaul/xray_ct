#ifndef VOLUME_H
#define VOLUME_H

#include <Eigen/Geometry>
#include <string>


class Volume {
private:
    Eigen::Vector3f _lowerLeft;
    Eigen::Vector3f _upperRight;
    Eigen::Vector3f _sp;

    Eigen::VectorXf _content;

public:
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp)
        : _lowerLeft(lowerLeft),
          _upperRight(upperRight),
          _sp(sp)
    {
    }

    Eigen::AlignedBox3f getBoundingBox() const
    {
        return Eigen::AlignedBox3f(_lowerLeft, _upperRight);
    }

    Eigen::Vector3f getSpacing() const
    {
        return _sp;
    }

    Eigen::Vector3i getNumVoxels() const
    {
        int x = floor((_lowerLeft[0]-_upperRight[0])/_sp[0]);
        int y = floor((_lowerLeft[1]-_upperRight[1])/_sp[1]);
        int z = floor((_lowerLeft[2]-_upperRight[2])/_sp[2]);
        return Eigen::Vector3i(x, y, z);
    }

    Eigen::VectorXf getContent() const
    {
        return _content;
    }
    void setContent(const Eigen::VectorXf& content)
    {
        _content = content;
    }

    // ... other useful methods ...

private:
    Eigen::AlignedBox3f boundingBox;
    Eigen::Vector3f spacing;

    Eigen::VectorXf content;
};

#endif // VOLUME_H
