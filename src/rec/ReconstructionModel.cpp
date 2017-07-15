#include "ReconstructionModel.h"

ReconstructionModel::ReconstructionModel () :
    _poses {std::make_shared<std::vector<AcquisitionPose>>()},
    _measurements {std::make_shared<Eigen::VectorXf>(0)},
    _cont {std::make_unique<NormalReconstructionContainer>(VolumeBase {}, *_poses)},
    _reconstruction {std::make_shared<Volume>()}
{
}
