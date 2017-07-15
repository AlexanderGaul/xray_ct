#include "TransferFunction.h"
#include <algorithm>

TransferFunction::LinearPiece::LinearPiece(float intensity0, float intensity1, int opacity0, int opacity1, QColor rgb)
    :
    _intensity0(intensity0),
    _intensity1(intensity1),
    _opacity0(opacity0),
    _opacity1(opacity1),
    _rgb(rgb)
{}

QColor TransferFunction::LinearPiece::apply(float intensity) const
{
    if(intensity < _intensity0)
    {
        // no opacity
        QColor color {_rgb};
        color.setAlpha(0);
        return color;
    }
    if(intensity > _intensity1)
    {
        // full opacity
        QColor color {_rgb};
        color.setAlpha(255);
        return color;
    }
    QColor color {_rgb};
    int opacity = static_cast<int>(_opacity0 + (_opacity1 - _opacity0) * (intensity - _intensity0) / (_intensity1 - _intensity0));
    color.setAlpha(std::max(std::min(opacity, 255), 0));
    return color;
}

TransferFunction::TransferFunction(LinearPiece function)
{
    _pieces.push_back(function);
}
TransferFunction::TransferFunction(std::vector<LinearPiece> pieces) 
    :
    _pieces(pieces)
{}

QColor TransferFunction::classify(float intensity) const
{
    QColor color {0, 0, 0, 0};
    for(std::vector<LinearPiece>::const_iterator it =  _pieces.begin(); it != _pieces.end(); it++)
    {
        color = it->apply(intensity);
        if(color.alpha() > 0)
        {
            return color;
        }
    }
    return color;
}

void TransferFunction::addPiece(LinearPiece function)
{
    _pieces.push_back(function);
}

std::vector<TransferFunction::LinearPiece>& TransferFunction::getPieces()
{
    return _pieces;
}
