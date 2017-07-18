#pragma once

#include <QColor>

#include "TransferFunction.h"
#include "Volume.h"

/**
 * The visualization model manages the current configuration
 * of the visualization.
 *
 * It is e.g. used to specify the color used to viusalize
 * the results of the reconstruction.
 * @brief The VisualizationModel class
 */
class VisualizationModel
{
private:
    QColor _color;

    Volume _volume;

public:
    VisualizationModel()
        : _color { QColor::fromRgb(255, 255, 255)}, //white
          _volume {}
    {        
    }

    void setVolume(Volume volume)
    {
        _volume = volume;
    }

    const Volume& volume() const
    {
        return _volume;
    }
};
