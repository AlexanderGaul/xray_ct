#ifndef VOLUME_H
#define VOLUME_H

#include <Eigen/Geometry>
#include <string>


class Volume {
public:
    Volume(Eigen::Vector3f lowerLeft, Eigen::Vector3f upperRight, Eigen::Vector3f sp);

    Eigen::AlignedBox3f getBoundingBox() const;
    Eigen::Vector3f getSpacing() const;

    Eigen::Vector3i getNumVoxels() const;

    Eigen::VectorXf getContent() const;
    void setContent(const Eigen::VectorXf& c);

    // ... other useful methods ...

private:
    Eigen::AlignedBox3f boundingBox;
    Eigen::Vector3f spacing;

    Eigen::VectorXf content;
};

#endif // VOLUME_H
