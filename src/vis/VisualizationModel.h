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
    VisualizationModel(TransferFunction transferFunction)
        : _color { QColor::fromRgb(255, 255, 255)}, //white
          _volume {},
          _transferFunction {transferFunction}
    {        
    }

    void setColor(QColor color)
    {
        _color = color;
        _transferFunction.setColor(0, color);
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
