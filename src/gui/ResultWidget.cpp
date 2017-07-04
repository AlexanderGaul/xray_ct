#include "ResultWidget.h"
#include <algorithm>

#include <iostream>

void ResultWidget::recalcProject() {
    std::vector<std::vector<float>> temp {};
    switch(_widgetStack.currentIndex()){
        case 0:
            temp = _model->forwardProjectSingle();
            break;
        case 1:
            temp = _model->forwardProjectFull();
            break;
    }
        
    if(temp.empty()){
        //fill the image with one black pixel
        _image = QImage {1,1, QImage::Format_Indexed8};
        _image.setColorTable(generateBlackWhiteColorTable());
        _image.setPixel(0,0,0);
        return;
    }
    int pixelY = temp.size();
    int pixelX = temp[0].size();
    
    //Only resize the image if its actually needed. Otherwise simply override the old image
    if(pixelY != _image.height() || pixelX != _image.width()){
        _image = QImage {pixelX, pixelY, QImage::Format_Indexed8};
        _image.setColorTable(generateBlackWhiteColorTable());
    }
    
    float max = maxPixel(temp);
    for(int y = 0; y < pixelY; ++y){
        for (int x = 0; x < pixelX; ++x){
            //There were some negative values, so to avoid bugs, currValue is at least zero
            float currValue = std::max(temp.at(y).at(x), 0.0f);
            int colorVal = 255*currValue/max;
            _image.setPixel(x, y, colorVal);
        }
    }
    update();
    }

ResultWidget::ResultWidget(AcquisitionModel* model, QWidget* parent)  
    : QWidget {parent}, _mainLayout {}, _widgetStack{}, 
    _drawWidgetSingle{}, _drawWidgetAll{}, 
    _state {DrawState::Single}, _model {model}, 
    _image {} {
    recalcProject();
        
    /*
     * Set the Background of the drawWidgets to black
    */
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    _drawWidgetSingle.setPalette(pal);
    _drawWidgetSingle.setAutoFillBackground(true);
    _drawWidgetAll.setPalette(pal);
    _drawWidgetAll.setAutoFillBackground(true);
    
    //see eventFilter
    _drawWidgetSingle.installEventFilter(this);
    _drawWidgetAll.installEventFilter(this);
        
    _widgetStack.addTab(&_drawWidgetSingle, "Latest Projection");
    _widgetStack.addTab(&_drawWidgetAll, "All Projections");
    _widgetStack.setCurrentIndex(0);
    _mainLayout.addWidget(&_widgetStack);
    QObject::connect(&_widgetStack, &QTabWidget::currentChanged, this,
                     &ResultWidget::tabChanged);
    this->setLayout(&_mainLayout);
}

bool ResultWidget::eventFilter(QObject* watched, QEvent* event) {
    if(event->type() == QEvent::Paint && ( watched == &_drawWidgetSingle ||
        watched == &_drawWidgetAll)){
        QWidget& widget = currWidget();
        QSize size = widget.size();
        
        //ensure that the image is always a rectangle
        int drawSize = std::min(widget.width(), widget.height());
             
        QRect drawRect{0, 0, drawSize, drawSize}; 
        
        QPainter painter {&widget};
        painter.drawImage(drawRect, _image);

        return true;
    } else {
        return QObject::eventFilter(watched, event);
    }
}

QWidget& ResultWidget::currWidget() {
    switch(_widgetStack.currentIndex()){
        case 0:
            return _drawWidgetSingle;
        case 1:
            return _drawWidgetAll;
        default:
            return _drawWidgetSingle;
       }
}

float ResultWidget::maxPixel(const std::vector< std::vector< float > >& projection) {
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
