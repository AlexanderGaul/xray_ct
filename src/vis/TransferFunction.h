#pragma once


#include <QColor>
#include <vector>

enum class ColorMixing : bool {
    Blend,
    Overlay
};

class TransferFunction
{
public:
    class LinearPiece
    {
    private:
        float _intensity0;
        float _intensity1;
        
        int _opacity0;
        int _opacity1;
        
        QColor _rgb;
    
    public:
        LinearPiece();
        LinearPiece(float intensity0, float intensity1, int opacity0, int opacity1, QColor rgb);
        
        QColor apply(float intensity);
    };
    
private:
    std::vector<LinearPiece> _pieces;

public:
    TransferFunction();
    TransferFunction(LinearPiece function);
    TransferFunction(std::vector<LinearPiece> pieces);
    
    QColor classify(float intensity);
    
    void addPiece(LinearPiece function);
    std::vector<LinearPiece>& getPieces();
};