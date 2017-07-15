#pragma once


#include <QColor>
#include <vector>

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
        LinearPiece(float intensity0, float intensity1, int opacity0, int opacity1, QColor rgb);
        
        QColor apply(float intensity) const;

        void setColor(QColor color)
        {
            _rgb = color;
        }

        QColor color() const
        {
            return _rgb;
        }
    };
    
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
        _pieces[index].color();
    }
};
