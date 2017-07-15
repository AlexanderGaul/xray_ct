#pragma once

#include <Eigen/Eigen>

using Position=Eigen::Vector3f;

class MPRModel
{
private:
    int _granularity;
    Position _t1;
    Position _t2;
    Position _t3;

public:
    MPRModel()
        : _granularity(5),
          _t1(Position(0,0,0)),
          _t2(Position(1,0,0)),
          _t3(Position(0,1,0))
    {
    }

    MPRModel(int granularity, Position t1, Position t2, Position t3)
        : _granularity(granularity),
          _t1(t1),
          _t2(t2),
          _t3(t3)
    {
    }

    int granularity() const
    {
        return _granularity;
    }

    const Position& t1() const
    {
        return _t1;
    }

    const Position& t2() const
    {
        return _t2;
    }

    const Position& t3() const
    {
        return _t3;
    }

    void setGranularity(int granularity)
    {
        _granularity = granularity;
    }

    void setT1(Position t1)
    {
        _t1 = t1;
    }

    void setT2(Position t2)
    {
        _t2 = t2;
    }

    void setT3(Position t3)
    {
        _t3 = t3;
    }
};
