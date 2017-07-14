#pragma once

#include <QColor>

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

public:
    VisualizationModel()
    {
        _color = QColor::fromRgb(0, 0, 0); // black
    }

    void setColor(QColor color)
    {
        _color = color;
    }

    const QColor& color() const
    {
        return _color;
    }
};
