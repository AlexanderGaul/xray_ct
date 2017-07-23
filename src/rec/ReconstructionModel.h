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
#include "Acquisition.h"

class ReconstructionModel
{
private:
    //the following two parameters can be only set in the constructor
    std::vector<AcquisitionPose> _poses;
    //For the error use a second _mesurements vector or overwrite the pointer, if you can
    //get it back. Don't remove the const!
    Eigen::VectorXf _measurements;
    
    std::unique_ptr<DataContainer> _cont;
    //the result of the _reconstruction
    bool _noisy;
    //To be able to easily remove the errors from the measurements they are stored extra
    Eigen::VectorXf _noise;
    
    std::shared_ptr<Volume> _reconstruction;
    
    /**
     * The reconstruction can be invalid if there is not enough information to restore there
     * volume or if lambda in the regularized reconstruction was too big
     * 
     * The result is that the reconstruction contains at least one NaN
     */
    bool _invalid;
    

public:
    
    std::unique_ptr<DataContainer> generateContainer(const VolumeBase& base, bool regularized, float lambda) {
        if(regularized){
            return std::make_unique<RegularizationContainer>(base, _poses, lambda);
        } else {
            return std::make_unique<NormalReconstructionContainer>(base, _poses);
        }
    }
    
    Eigen::VectorXf getMeasurements(){
        return _noisy ? _measurements + _noise : _measurements;
    }
    
    void recalcReconstruction(int iterations){
        _reconstruction = std::make_shared<Volume>(*_reconstruction, CG::conjugateGradient(iterations, *_cont, getMeasurements()));
        _invalid = _reconstruction->containsNan();
    }
    
    const VolumeBase& getVolumeBase(){
        return *_reconstruction;
    }
    
    Eigen::VectorXf generateNoise(double noise){
        if (noise == 0.0) {
            return Eigen::VectorXf::Zero(_measurements.size());
        } else {
            static const unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()
                        % std::numeric_limits<unsigned int>::max());
            static std::mt19937 gen {seed};
            static std::normal_distribution<float> dist {};
            
            Eigen::VectorXf noiseVec {_measurements.size()};
            const float minMaxDiff = _measurements.maxCoeff() - _measurements.minCoeff();
            for(int i = 0; i < noiseVec.size(); ++i){
                noiseVec[i] = noise * minMaxDiff * dist(gen);
            }
            return noiseVec;
        }
        
    }
    
    bool invalid(){
        return _invalid;
    }
    
public:
    ReconstructionModel (bool regularized, float lambda, bool noisy, float noise, int cgIterations, Acquisition&& acq)
    :
                _poses {std::move(acq.poses)},
                _measurements {std::move(acq.measurements)},
                _cont {generateContainer(acq.volBase, regularized, lambda)},
                _noisy {noisy}, _noise {generateNoise(noise)},
                _reconstruction {std::make_shared<Volume>(acq.volBase, CG::conjugateGradient(cgIterations, *_cont, getMeasurements()))}, _invalid {_reconstruction->containsNan()}
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
    void changeReconstructionParams(bool regularized, float lambda, int iterations)
    {
        _cont = generateContainer(*_reconstruction, regularized, lambda);
        recalcReconstruction(iterations);
    }
    
    void changeNoise(bool noisy, float noise, int iterations){
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
