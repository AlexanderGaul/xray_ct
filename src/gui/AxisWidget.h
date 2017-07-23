#ifndef AXISWIDGET_H
#define AXISWIDGET_H

#include <array>

#include <QWidget>
#include <QPainter>
#include <Eigen/Geometry>

#include "MPRModel.h"

/**
 * Shows the axis of the coordinate system of the cutting plane
 */
class AxisWidget : public QWidget
{
    Q_OBJECT
private:
    const MPRModel& _model; 
    
    
public:
    AxisWidget(const MPRModel& model) : _model {model} {
        connect(&_model, &MPRModel::redraw, this, &AxisWidget::redraw
        );
    }

    void paintEvent(QPaintEvent *) override;
    

public slots:
    
    void redraw (){
        update();
    }
};

#endif // AXISWIDGET_H
