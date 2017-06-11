#pragma once
#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>

#include <QFile>
#include <QTextStream>

#include "EDFhandler.h"
#include "Volume.h"

class AcquisitionModel
{
private:
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
    }

private:
    bool _filled;
    Volume _volume;

};

#endif // ACQUISITIONMODEL_H
