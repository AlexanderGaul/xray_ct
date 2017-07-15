#include "MPRModel.h"

MPRModel::MPRModel()
    : _granularity(10),
      _t1(Position(0,0,0)),
      _t2(Position(1,0,0)),
      _t3(Position(0,1,0))
{
}

MPRModel::MPRModel(int granularity, Position t1, Position t2, Position t3)
    : _granularity(granularity),
      _t1(t1),
      _t2(t2),
      _t3(t3)
{
}

int MPRModel::granularity() const
{
    return _granularity;
}

const Position& MPRModel::t1() const
{
    return _t1;
}

const Position& MPRModel::t2() const
{
    return _t2;
}

const Position& MPRModel::t3() const
{
    return _t3;
}

void MPRModel::setGranularity(int granularity)
{
    _granularity = granularity;
}

void MPRModel::setT1(Position t1)
{
    _t1 = t1;
}

void MPRModel::setT2(Position t2)
{
    _t2 = t2;
}

void MPRModel::setT3(Position t3)
{
    _t3 = t3;
}
