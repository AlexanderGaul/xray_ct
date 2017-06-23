#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <limits>
#include <algorithm>

#include <QWidget>
#include <QPainter>
#include <QBrush>

#include "AcquisitionModel.h"

/*
 * Draws the forward Projection of the current aquisition pose
 */
class ResultWidget : public QWidget
{
public:
    ResultWidget(AcquisitionModel *model, QWidget *parent = nullptr) 
    : QWidget {parent}, _model {model}, _currProjection {_model->forwardProject()} {
        
        /*
         * Set the Background to black
         */
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::black);
        this->setAutoFillBackground(true);
        this->setPalette(pal);
    }
public slots:
    /*
     * When the pose (or the volume) was updated, the forward Projection has to be 
     * recalculated and redrawn
     */
    void recalcProject(){
        _currProjection = _model->forwardProjectFull();

        // we know that projection contains a linearized square matrix
//        std::vector<float> projection = _model->forwardProject();
//        int n = sqrt(projection.size());

//        _currProjection.clear();
//        _currProjection.reserve(n);
//        for(int i = 0; i < n; i++)
//        {
//            std::vector<float> temp;
//            temp.reserve(n);
//            for(int j = 0; j < n; j++)
//            {
//                temp.push_back(projection[i*n + j]);
//            }
//            _currProjection.push_back(temp);
//        }
        repaint();
    }
    
protected slots:
    
    /*
     * The white color always represents the maximum pixel value (because it is unbounded).
     * A value of 0 is black. All other pixel values are distributed in between according
     * to their value (the higher the value, the whiter the pixel)
     */
    void paintEvent(QPaintEvent *) override{
        QSize size = this->size();
        std::size_t pixelX = _currProjection[0].size();
        std::size_t pixelY = _currProjection.size();
        std::size_t pixelWidth = size.width()/pixelX;
        std::size_t pixelHeight = size.height()/pixelY;
        float max = maxPixel(_currProjection);
        QPainter painter {this};
        QColor currColor {};
        for(int y = 0; y < pixelY; ++y){
            for (int x = 0; x < pixelX; ++x){
                //There were some negative values, so to avoid bugs, currValue is at least zero
                float currValue = std::max(_currProjection[y][x], 0.0f);
                int colorVal = 255*currValue/max;
                currColor.setRgb(colorVal, colorVal, colorVal);
                painter.fillRect(x*pixelWidth, y*pixelHeight, pixelWidth, pixelHeight, currColor);
            }
        }
    }
    
private:
    
    /*
     * Find the maximum pixel value from the forward Projection.
     */
    float maxPixel(const std::vector<std::vector<float>>& projection){
        float max = std::numeric_limits<float>::min();
        for(const auto& vec : projection){
            for(const float f : vec){
                if(f > max){
                    max = f;
                }
            }
        }
        return max;
    }
    
    AcquisitionModel *_model;
    std::vector<std::vector<float>> _currProjection;
};

#endif // RESULTWIDGET_H
