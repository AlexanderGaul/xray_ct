#pragma once

#include <iostream>
#include <vector>

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QSlider>
#include <QWheelEvent>
#include <QWidget>

#include "Timer.h"
#include "Vec3D.h"
#include "Volume.h"

class SliceWidget : public QWidget
{
    Q_OBJECT
private:
    /// reference to the raw data that is actually managed in the acquistion model
    const Volume& _volume;
    int _currSlice;

    int _status;

public:
    SliceWidget(const Volume& volume)
        : _volume(volume), _currSlice(0), _status(2)
    {
    }

    virtual
    void paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);

        const Vec3D<float>& content = _volume.content();

        float maxColor = _volume.maxEntry();
        float colorCoeff = 255.0/maxColor;
        // TODO: reduce duplication
        if(_status == 2)
        {
            int pixelWidth = 1.0*width()/(content.sizeX());
            int pixelHeight = 1.0*height()/(content.sizeY());

            for(int i = 0; i< content.sizeX(); ++i)
            {
                for(int j = 0; j<content.sizeY(); ++j)
                {
                    int curr = (content.get(i,j,_currSlice)) * colorCoeff;

                    QColor color = QColor::fromRgb(curr, curr, curr);
                    painter.fillRect(QRect(i*pixelWidth, j*pixelHeight, pixelWidth, pixelHeight), QBrush(color));
                }
            }
            return;
        }
        if(_status == 1) // Y
        {
            int pixelWidth = 1.0*width()/(content.sizeX());
            int pixelHeight = 1.0*height()/(content.sizeZ());
            for(int i = 0; i< content.sizeX(); ++i)
            {
                for(int j = 0; j<content.sizeZ(); ++j)
                {
                    int curr = (content.get(i,_currSlice,j)) * colorCoeff;

                    QColor color = QColor::fromRgb(curr, curr, curr);
                    painter.fillRect(QRect(i*pixelWidth, j*pixelHeight, pixelWidth, pixelHeight), QBrush(color));
                }
            }
            return;
        }
        if(_status == 0) // X
        {
            int pixelWidth = 1.0*width()/(content.sizeY());
            int pixelHeight = 1.0*height()/(content.sizeZ());
            for(int i = 0; i< content.sizeY(); ++i)
            {
                for(int j = 0; j<content.sizeZ(); ++j)
                {
                    int curr = (content.get(_currSlice,i,j)) * colorCoeff;

                    QColor color = QColor::fromRgb(curr, curr, curr);
                    painter.fillRect(QRect(i*pixelWidth, j*pixelHeight, pixelWidth, pixelHeight), QBrush(color));
                }
            }
        }

    }

    void wheelEvent(QWheelEvent* event)
    {
        if(event->delta() == 0)
        {
            // no real motion! do nothing!
            return;
        }
        if(event->delta() < 0)
        {
            // go back
            if(_currSlice > 0)
            {
                _currSlice--;
                emit sliceChanged();
                repaint();
            }
        }
        else
        {
            // go forward
            if(_currSlice < slices()-1)
            {
                _currSlice++;
                emit sliceChanged();
                repaint();
            }
        }
    }

    int currSlice()
    {
        return _currSlice;
    }

    int slices()
    {
        switch(_status)
        {
            case 0:
                return _volume.content().sizeX();
            case 1:
                return _volume.content().sizeY();
            default:
                return _volume.content().sizeZ();
        }

        return _volume.content().sizeZ();
    }

signals:
    void sliceChanged();

public slots:
    void updateStatus(int newStatus)
    {
        newStatus--; //zero-indexed slice widget
        if(newStatus != _status)
        {
            _status = newStatus-1;
            _currSlice = 0;
        }
        repaint();
    }
};
