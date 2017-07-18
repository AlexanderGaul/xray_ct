#pragma once

#include <QColor>

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

    void setIntensity0(float intensity0)
    {
        _intensity0 = intensity0;
    }

    void setIntensity1(float intensity1)
    {
        _intensity1 = intensity1;
    }
};
