#pragma once
#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>
#include <memory>
#include <array>
#include <cmath>

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "AcquisitionPose.h"
#include "EDFhandler.h"
#include "ForwardProjectionOperator.h"
#include "RayTracing.h"
#include "Volume.h"
#include "Acquisition.h"

/*
 * Represents an axis of rotation.
 * If something is rotated aroundthe z-axis it is rotated in the x-y-plane
 */
enum class RotationAxis : bool {
    Z,
    X
};

class AcquisitionModel : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief AcquisitionModel. Creates an acquisition model and loads
     * an EDF image from the given path.
     * @param path - file system path determining location of EDF image.
     */
    AcquisitionModel(std::string path);

    /**
     * @brief loadImage. Loads a new EDF image into the acquisition model.
     * @param path - file system path determining location of EDF image.
     */
    void loadImage(std::string path);
    
    /**
     * The next few methods offer an interface to pose that is shown in the the gui.
     * See AcquisitionPose for documentation.
     */
    void updateRotation(RotationAxis axis, float angle);
    
    /**
     * Returns the corners of the detector in the following order:
     * UpperLeft, LowerLeft, LowerRight, UpperRight
     */
    std::array<Eigen::Vector3f, 4> getDetector() const;

    Eigen::Vector2i getDetectorSize() const;

    Eigen::Vector3f getSourcePosition() const;
    
    Eigen::Vector3f getPixelCenter(int i, int j) const;
    
    /*
     * returns the bouding Box of the volume
     */
    Eigen::AlignedBox3f getBoundingBox() const;
  
    /**
     * @brief writeImage. Writes an EDF iamge into a specified path
     * @param path - file system path determining the destination of the image.
     */
    void writeImage(std::string path) const;
    
    /*
     * Deletes all stored poses
     */
    void clearPoses();
    
    /*
     * Deletes the current Pose. If its the last one reset it to the default pose.
     */
    void deletePose();
    
    /*
     * Saves the current pose, so that a new one can be defined by the user
     */
    void savePose();
    
    
    /*
     * Is used for the pose generation
     * 
     * The same as below only that there are now to rotation axis. So for every possible rotation state
     * around the first axis there are count2 rotations states around the second axis, so that
     * in the end there are count1*count2 generated Poses.
     */
    void generatePoses(int count1, int count2){
        _poses->pop_back();
        float angleX = 2.f * M_PI / count1;
        float angleZ = 2.f * M_PI / count2;
        
        for(int i = 0; i < count1; ++i){
            for(int p = 0; p < count2; ++p){
                AcquisitionPose pose {_volume.getBoundingBox()};
                pose.setRotation(i*angleX, p*angleZ);
                _poses->push_back(std::move(pose));
            }
        }
        
        updateProjection();
    }
    
    /*
     * 
     * 
     * There is one defined rotation axis (currently only the X-axis)
     * 
     * The full rotation of 2*PI is diveded into count1 parts. So that for the i-th of the count1 pose
     * its rotation is defined as i*2*PI/count1 around the defined axis.
     */
    void generatePoses(int count1){
        _poses->pop_back();
        float angleX = 2.f * M_PI / count1;
        
        for(int i = 0; i < count1; ++i){
            AcquisitionPose pose {_volume.getBoundingBox()};
            pose.setRotation(i*angleX);
            _poses->push_back(std::move(pose));
        }
        updateProjection();
    }
    
    /**
     * Returns a reference to the volume (containing the raw data).
     * @brief volume
     * @return
     */
    const Volume& volume() const
    {
        return _volume;
    }
    
    /*
     * returns only the last projection
     */
    std::pair<int, const Eigen::VectorXf> getLastProj() {
        const AcquisitionPose currAcq = currPoseChecked();
        const int totalSize = currAcq.getPixelCount();
        const int offset = _measurements->size() - totalSize;
        
        
        Eigen::VectorXf proj {totalSize};
        for(int p = 0; p < totalSize; ++p){
            proj[p] = (*_measurements)[offset + p];
        }
        return std::make_pair(currAcq.getPixelVertical(), proj);
    }
    
    /*
     * returns a pair of the number of rows (=rays) in the _measurements arrey
     * and the measurements themselves
     * This may not be the safest way to acess the vector for painting it but
     * probably the fastest.
     * 
     * TODO right now the Projection vector is copied unecessarily but a refernce
     * didn't work. Shouldn't be a problem most of the time; can be fixed later
     */
    std::pair<int, const Eigen::VectorXf> getProj() {
        return std::make_pair(_poses->size(), *_measurements); 
    }
    
    const Eigen::VectorXf& getProjection(){
        return *_measurements;
    }
    
    /*
     * returns the acquisition information needed for the reconstruction if requested
     */
    Acquisition getAcq() const {
        return Acquisition {_volume, _poses, _measurements};
    }

signals:  
    //emited when the acquistion pose changes (because of user action)
    void poseChanged();
private:
    bool checkIfVolumeFitsBlackBox() const;
    
    /*
     * adds a new default AcquisitionPose
     */
    void addDefaultPose(){
        _poses->push_back(AcquisitionPose {getBoundingBox()});
        
        updateLastProjection();
    }
    
    /**
     * Should be called when new poses were added
     * Updates the _measurements vector
     */
    void updateProjection(){
        _measurements = std::make_shared<Eigen::VectorXf>(ForwardProjectionOperator::forwardProj(_volume, *_poses, _volume.content().rawVec()));
        emit poseChanged();
    }
    
    /*
     * This is somewaht useless currently. I will use it for optimisation later.
     * Just use updateProjection for now instead
     */
    void updateLastProjection(){
        const int rayCount = currPoseChecked().getPixelCount();
        const Eigen::VectorXf proj = ForwardProjectionOperator::forwardProj(volume(), currPoseChecked());
        const int offset = _measurements->size() - rayCount;
        assert(rayCount == proj.size());
        
        for(int x = 0 ; x < rayCount; ++x) {
            (*_measurements)[offset + x] = proj[x];
        }
        emit poseChanged();
    }
    
    /*
     * The pose that is shown on the screen and which can be modified, is the last pose
     * inserted into the poses stack.
     * It is called the current Pose.
     */
    AcquisitionPose& currPose();
    
    const AcquisitionPose& currPose() const;
    
    AcquisitionPose& currPoseChecked();
    
    const AcquisitionPose& currPoseChecked() const;
    
    const Eigen::Vector3f FIXED_BOX_SIZE = Eigen::Vector3f(0.15, 0.15, 0.25);
    bool _filled;
    Volume _volume;
    /*
     * Is effectively a stack. May later be replaced with std::stack TODO
     * Stores older poses that were created by the user, so that a ForwardProjection on 
     * more than one pose can be calculated.
     * There has to be alway one element in this stack, which is the one shown in the gui.
     */
    std::shared_ptr<std::vector<AcquisitionPose>> _poses;
    
    std::shared_ptr<Eigen::VectorXf> _measurements;

};

#endif // ACQUISITIONMODEL_H
