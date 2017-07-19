#include "MPRModel.h"

MPRModel::MPRModel()
    : _granularity(10),
      _distance(0.01),
      _normal(1,0,0),
      _transferFunction(TransferFunction(LinearPiece(0, 100, 0, 255, QColor::fromRgb(255,255,255)))),
      _color(QColor::fromRgb(255,255,255))
{
}

MPRModel::MPRModel(int granularity, float distance, Eigen::Vector3f normal,
                   TransferFunction transferFunction)
    : _granularity(granularity),
      _distance(distance),
      _transferFunction(transferFunction),
      _color(transferFunction.color(0))
{
}

int MPRModel::granularity() const
{
    return _granularity;
}

void MPRModel::setGranularity(int granularity)
{
    _granularity = granularity;
}
