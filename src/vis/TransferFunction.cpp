#include "TransferFunction.h"
#include <algorithm>


TransferFunction::LinearPiece::LinearPiece() :
    LinearPiece(0, 100, 0, 255, QColor(255, 255, 255))
{}
TransferFunction::LinearPiece::LinearPiece(float intensity0, float intensity1, int opacity0, int opacity1, QColor rgb) 
    :
    _intensity0(intensity0),
    _intensity1(intensity1),
    _opacity0(opacity0),
    _opacity1(opacity1),
    _rgb(rgb)
{}

QColor TransferFunction::LinearPiece::apply(float intensity)
{
    if(_intensity0 <= intensity && intensity <= _intensity1)
    {
        QColor color {_rgb};
        int opacity = static_cast<int>(_opacity0 + (_opacity1 - _opacity0) * (intensity - _intensity0) / (_intensity1 - _intensity0));
        color.setAlpha(std::max(std::min(opacity, 255), 0));
        return color;
    }
}


TransferFunction::TransferFunction()
{
    _pieces.push_back(LinearPiece());
}
TransferFunction::TransferFunction(LinearPiece function)
{
    _pieces.push_back(function);
}
TransferFunction::TransferFunction(std::vector<LinearPiece> pieces) 
    :
    _pieces(pieces)
{}

QColor TransferFunction::classify(float intensity)
{
    QColor color {0, 0, 0, 0};
    for(std::vector<LinearPiece>::iterator it =  _pieces.begin(); it != _pieces.end(); it++)
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