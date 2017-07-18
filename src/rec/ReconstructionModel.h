#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <random>


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
    bool _noisy;
    //To be able to easily remove the errors from the measurements they are stored extra
    Eigen::VectorXf _noise;
    
    std::shared_ptr<Volume> _reconstruction;
    

    
    
    std::unique_ptr<DataContainer> generateContainer(const VolumeBase& base, bool regularized, float lambda) {
        if(regularized){
            return std::make_unique<RegularizationContainer>(base, *_poses, lambda);
        } else {
            return std::make_unique<NormalReconstructionContainer>(base, *_poses);
        }
    }
    
    Eigen::VectorXf getMeasurements(){
        return _noisy ? *_measurements + _noise : *_measurements;
    }
    
    void recalcReconstruction(int iterations){
        _reconstruction = std::make_shared<Volume>(*_reconstruction, CG::conjugateGradient(iterations, *_cont, getMeasurements()));
    }
    
    Eigen::VectorXf generateNoise(double noise){
        if (noise == 0.0) {
            return Eigen::VectorXf::Zero(_measurements->size());
        } else {
            auto seed = std::chrono::system_clock::now().time_since_epoch().count();
            //TODO
            static std::mt19937 gen {1};
            static std::normal_distribution<float> dist {};
            
            Eigen::VectorXf noiseVec {_measurements->size()};
            const float minMaxDiff = _measurements->maxCoeff() - _measurements->minCoeff();
            for(int i = 0; i < _noise.size(); ++i){
                noiseVec[i] = noise * minMaxDiff * dist(gen);
            }
            return noiseVec;
        }
        
    }
    
public:
    /*
     * This constructor is used when the poses and the measurements change
     */
    ReconstructionModel ();

    ReconstructionModel (bool regularized, float lambda, bool noisy, float noise, int cgIterations,  const VolumeBase& base, 
                    const std::shared_ptr<const std::vector<AcquisitionPose>>& poses, const std::shared_ptr<const Eigen::VectorXf>& measurements)
    :
                _poses {poses},
                _measurements {measurements},
                _cont {generateContainer(base, regularized, lambda)},
                _noisy {noisy}, _noise {generateNoise(noise)},
                _reconstruction {std::make_shared<Volume>(base, CG::conjugateGradient(cgIterations, *_cont, getMeasurements()))}
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
    void changeReconstructionParams(bool regularized, float lambda, bool noisy, float noise, int iterations)
    {
        _cont = generateContainer(*_reconstruction, regularized, lambda);
        _noisy = noisy;
        if(_noisy){
            _noise = generateNoise(noise);
        }
        recalcReconstruction(iterations);
    }
    
    const Vec3D<float>& getContent() {
        return _reconstruction->content();
    }
};
