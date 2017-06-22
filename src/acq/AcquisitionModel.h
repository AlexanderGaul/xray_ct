#pragma once
#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>

#include <QFile>
#include <QTextStream>

#include "EDFhandler.h"
#include "Volume.h"
#include "AcquisitionPose.h"
#include "RayTracing.h"

class AcquisitionModel
{
private:
    bool checkIfVolumeFitsBlackBox()
    {
        // check if the volume fits the black box
        Eigen::Vector3i voxels = _volume.getNumVoxels();
        Eigen::Vector3f spacing = _volume.getSpacing();

        for(int i = 0; i<3; i++)
        {
            if(voxels[i] * spacing[i] > FIXED_BOX_SIZE[i])
            {
                return false;
            }
        }
        return true;
    }
    
    

public:
    AcquisitionModel()
        : _volume(Eigen::Vector3f(-1,-1,-1),
                  Eigen::Vector3f(-1,-1,-1),
                  Eigen::Vector3f(-1,-1,-1)
                  )
    {
        _filled = false;
    }

    AcquisitionModel(std::string path)
        : _volume(EDFHandler::read(path))
    {
        _filled = true;
    }

    void loadImage(std::string path)
    {
        _volume = EDFHandler::read(path);
        if(!checkIfVolumeFitsBlackBox())
        {
            throw std::logic_error("the specified volume does not fit the black box!");
        }
    }
    
    std::vector<std::vector<float>> forwardProject() {
        std::vector<float> temp {};
        temp.reserve(pose.getPixelHorizontal());
        
        std::vector<std::vector<float>> ret (pose.getPixelVertical(), temp);
        for(int y = 0; y < pose.getPixelVertical(); ++y){
            auto& currRow = ret[y];
            for(int x = 0; x < pose.getPixelHorizontal(); ++x){
                currRow[x] = RayTracing::forwardProject(_volume, pose.getRay(x, y));
            }
        }
        
        return ret;
    }

private:
    
    const Eigen::Vector3f FIXED_BOX_SIZE = Eigen::Vector3f(0.15, 0.15, 0.25);
    bool _filled;
    Volume _volume;
    AcquisitionPose pose;

};

#endif // ACQUISITIONMODEL_H
