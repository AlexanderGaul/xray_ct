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

    TransferFunction _transferFunction;

public:
    VisualizationModel()
        : _color { QColor::fromRgb(0, 0, 0)}, // black
          _volume {},
          _transferFunction {}
    {        
    }

    void setColor(QColor color)
    {
        _color = color;
    }

    const QColor& color() const
    {
        return _color;
    }

    void setVolume(Volume volume)
    {
        _volume = volume;
    }

    const Volume& volume() const
    {
        return _volume;
    }

    void setTransferFunction(TransferFunction transferFunction)
    {
        _transferFunction = transferFunction;
    }

    const TransferFunction& transferFunction() const
    {
        return _transferFunction;
    }
};
