#pragma once

#include <vector>
#include <memory>
#include <memory>

#include <Eigen/Eigen>

#include "Volume.h"
#include "RayTracing.h"
#include "CG.h"
#include "RegularizationContainer.h"
#include "NormalReconstructionContainer.h"

class ReconstructionModel
{
private:
    //the following two parameters can be only set in the constructor
    std::shared_ptr<const std::vector<AcquisitionPose>> _poses;
    //For the error use a second _mesurements vector or overwrite the pointer, if you can
    //get it back. Don't remove the const!
    std::shared_ptr<const Eigen::VectorXf> _measurements;
    
    std::unique_ptr<DataContainer> _cont;
    //the result of the _reconstruction
    std::shared_ptr<Volume> _reconstruction;
    
    std::unique_ptr<DataContainer> generateContainer(const VolumeBase& base, bool regularized, float lambda) {
        if(regularized){
            return std::make_unique<RegularizationContainer>(base, *_poses, lambda);
        } else {
            return std::make_unique<NormalReconstructionContainer>(base, *_poses);
        }
    }
    
public:
    /*
     * This constructor is used when the poses and the measurements change
     */
    ReconstructionModel () : 
        _poses {std::make_shared<std::vector<AcquisitionPose>>()}, 
        _measurements {std::make_shared<Eigen::VectorXf>(0)}, 
        _cont {std::make_unique<NormalReconstructionContainer>(VolumeBase {}, *_poses)}, 
        _reconstruction {std::make_shared<Volume>()}
    {
    }
    
    ReconstructionModel (
        bool regularized, 
        float lambda, 
        int cgIterations,  
        const VolumeBase& base, 
        const std::shared_ptr<const std::vector<AcquisitionPose>>& poses, 
        const std::shared_ptr<const Eigen::VectorXf>& measurements)
    :   _poses {poses}, 
        _measurements {measurements}, 
        _cont {generateContainer(base, regularized, lambda)}, 
        _reconstruction {std::make_shared<Volume>(base, CG::conjugateGradient(cgIterations, *_cont, *_measurements))}
    {
    }
    
    std::shared_ptr<const Volume> getRec() const{
        return _reconstruction;
    }
    
    
    const Volume& rec(){
        return *_reconstruction;
    }
    
public slots:
    
    /*
     * This is used when only one of the parameters change.
     */
    void recalcVolume(bool regularized, int iterations, float lambda){
        _cont = generateContainer(*_reconstruction, regularized, lambda);
        *_reconstruction = Volume {*_reconstruction, CG::conjugateGradient(iterations, *_cont, *_measurements)};
    }
    
    const Vec3D<float>& getContent() {
        return _reconstruction->content();
    }
};
