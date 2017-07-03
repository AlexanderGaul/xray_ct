#pragma once

#include <iostream>
#include <vector>

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QSlider>
#include <QWheelEvent>
#include <QWidget>

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
        : _volume(volume), _currSlice(volume.content().sizeZ()/2), _status(2)
    {
    }

    virtual
    void paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);

        const Vec3D<float>& content = _volume.content();


        // TODO: reduce duplication
        if(_status == 2)
        {
            int pixelWidth = 1.0*width()/(content.sizeX());
            int pixelHeight = 1.0*height()/(content.sizeY());
            for(int i = 0; i< content.sizeX(); ++i)
            {
                for(int j = 0; j<content.sizeY(); ++j)
                {
                    int curr = (content.get(i,j,_currSlice));

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
                    int curr = (content.get(i,_currSlice,j));

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
                    int curr = (content.get(_currSlice,i,j));

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
            if(_currSlice < _volume.content().sizeZ()-1)
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
        return _volume.content().sizeZ();
    }

signals:
    void sliceChanged();

public slots:
    void updateStatus(int newStatus)
    {
        _status = newStatus-1;
        std::cout << _status << std::endl;
        repaint();
    }
};
