#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <limits>
#include <algorithm>

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QTabWidget>
#include <QObject>
#include <QVBoxLayout>
#include <QEvent>

#include "AcquisitionModel.h"

enum class DrawState{
    Single = 0,
    All = 1,
    Angle = 2
};

/*
 * Draws the forward Projection of the current aquisition pose
 */
class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    ResultWidget(AcquisitionModel *model, QWidget *parent = nullptr);
public slots:
    
    /*
     * When the pose (or the volume) was updated, the forward Projection has to be 
     * recalculated and redrawn
     */
    void recalcProject();
    
protected slots:
    
    void paintEvent(QPaintEvent *event) override{
        //for some reason update wasn't called automatically
        //even though the drawWidgets are children of this widget
        currWiget().update();
    }
    
    /*
     * offers parameter compatibility with the signal of the tabWidget
     */
    void tabChanged(int){
        recalcProject();
    }
    
    /*
     * Does the drawing.
     * paintEvent can't be used, since it it isn't allowed to draw on children, so this
     * is the workaround that avoids creating a subclass
     * To be get the events of the children this has to be registered in the 
     * children as listener (see constructor)
     */
    bool eventFilter(QObject * watched, QEvent * event) override;
    
private:
    
    /*
     * Find the maximum pixel value from the forward Projection.
     * This pixel will always be the white pixel.
     */
    float maxPixel(const std::vector<std::vector<float>>& projection);
    
    /*
     * returns the widget that is currently shown
     * used to see which widget has to be updated
     */
    QWidget& currWiget();
    
    QVBoxLayout _mainLayout;
    QTabWidget _widgetStack;
    QWidget _drawWidgetSingle;
    QWidget _drawWidgetAll;
    QWidget _drawWidgetAngle;
    DrawState _state;
    AcquisitionModel *_model;
    std::vector<std::vector<float>> _currProjection;
};

#endif // RESULTWIDGET_H
