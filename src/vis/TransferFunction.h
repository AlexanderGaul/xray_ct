#pragma once

#include <memory>
#include <vector>

#include <QColor>

#include "LinearPiece.h"

enum class ColorMixing : bool {
    Blend,
    Overlay
};

/**
 * Implementation of simple ramp transfer function.
 *
 * A transfer function is able to map a given intensity
 * to a color.
 * These color gets more opaque at higher intensity values.
 * @brief The TransferFunction class
 */
class TransferFunction
{
private:
    std::vector<LinearPiece> _pieces;

public:
    TransferFunction(LinearPiece function);
    TransferFunction(std::vector<LinearPiece> pieces);
    
    QColor classify(float intensity) const;
    
    void addPiece(LinearPiece function);
    std::vector<LinearPiece>& getPieces();

    void setColor(int index, QColor color)
    {
        _pieces[index].setColor(color);
    }

    QColor color(int index) const
    {
        return _pieces[index].color();
    }

    void setRange(int index, float from, float to)
    {
        _pieces[index].setIntensity0(from);
        _pieces[index].setIntensity1(to);
    }
};
