#pragma once

#include <iostream>
#include <vector>

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QSlider>
#include <QWheelEvent>
#include <QWidget>
#include <QImage>

#include "Vec3D.h"
#include "Volume.h"
#include "ReconstructionModel.h"
#include "Acquisition.h"

class SliceWidget : public QWidget
{
    Q_OBJECT
private:
    int _currSlice;
    int _status;
    ReconstructionModel _model;
    QImage _image;

public:
    SliceWidget() : _currSlice(0), _status(2), _model {ReconstructionModel {}}, _image {} {
    }

    virtual
    void paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);

        const Vec3D<float>& content = _model.getContent();
        if(content.totalSize() == 0){
            return;
        }

        float maxColor = _model.rec().maxEntry();
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
                std::cout << _model.rec().content().sizeX() << std::endl;
                return _model.rec().content().sizeX();
            case 1:
                return _model.rec().content().sizeY();
            default:
                return _model.rec().content().sizeZ();
        }
    }
    
    /*
     * called when the user requests a new Acquisition.
     * 
     * regularized defines which DataContainer to use
     * lambda is the lambda for the regularized Container, it is only used when
     * regularized is actually true, otherwise it is discarded
     */
    void setAcq(bool regularized, float lambda, int cgIterations, Acquisition&& acq){
        _model = ReconstructionModel {regularized, lambda, cgIterations, std::move(acq.volBase), std::move(acq.poses), std::move(acq.measurements)};
        update();
        //Important reset, if the volume boundaries would recParamChanged
        _currSlice = 0;
        emit sliceChanged();
    }
    
    /*
     * called one of the parmeters is changed by the user in the ReconstructionView
     * 
     * param. description see above
     */
    void recParamChanged(bool regularized, float lambda, int cgIterations){
        _model.recalcVolume(regularized, lambda, cgIterations);
        update();
        _currSlice = 0;
        emit sliceChanged();
    }
    
    std::shared_ptr<const Volume> getRec(){
        return _model.getRec();
    }

    int iterations()
    {
        return _model.iterations();
    }

signals:
    void sliceChanged();

public slots:

    void setIterations(int iterations)
    {
        _model.setIterations(iterations);
        repaint();
    }

    void updateStatus(int newStatus)
    {
        newStatus--; //zero-indexed slice widget
        if(newStatus != _status)
        {
            _status = newStatus;
            _currSlice = 0;
        }
        repaint();
    }
};
