#pragma once
#ifndef MPRMODEL_H
#define MPRMODEL_H

#include <Eigen/Eigen>

#include "TransferFunction.h"

typedef Eigen::Vector3f Position;

/**
 * The MPRModel contains all data needed only for the
 * MPR visualization process.
 * @brief The MPRModel class
 */
class MPRModel
{
private:
    int _granularity;
    Position _t1;
    Position _t2;
    Position _t3;
    Position _t4;

    TransferFunction _transferFunction;

public:
    MPRModel();

    MPRModel(int granularity, Position t1, Position t2, Position t3,
             TransferFunction transferFunction);

    int granularity() const;

    const Position& t1() const;
    const Position& t2() const;
    const Position& t3() const;
    const Position& t4() const;

    void setGranularity(int granularity);
    void setT1(Position t1);
    void setT2(Position t2);
    void setT3(Position t3);
    void setT4(Position t4);

    void setTransferFunction(TransferFunction transferFunction)
    {
        _transferFunction = transferFunction;
    }

    const TransferFunction& transferFunction() const
    {
        return _transferFunction;
    }

    void setColor(QColor color)
    {
        _transferFunction.setColor(0, color);
    }
};

#endif //MPRMODEL_H
