#include "ResultWidget.h"
#include <algorithm>

void ResultWidget::recalcProject() {
        switch(_widgetStack.currentIndex()){
            case 0:
                _currProjection = _model->forwardProjectSingle();
                break;
            case 1:
                _currProjection = _model->forwardProjectFull();
                break;
            case 2:
                _currProjection = _model->forwardProjectAngle();
                 break;
        }
        repaint();
    }

ResultWidget::ResultWidget(AcquisitionModel* model, QWidget* parent)  
    : QWidget {parent}, _mainLayout {}, _widgetStack{}, 
    _drawWidgetSingle{}, _drawWidgetAll{}, _drawWidgetAngle{},  
    _state {DrawState::Single}, _model {model}, 
    _currProjection {_model->forwardProjectSingle()} {
        
        /*
         * Set the Background of the drawWidgets to black
         */
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::black);
        _drawWidgetSingle.setPalette(pal);
        _drawWidgetSingle.setAutoFillBackground(true);
        _drawWidgetAll.setPalette(pal);
        _drawWidgetAll.setAutoFillBackground(true);
        _drawWidgetAngle.setPalette(pal);
        _drawWidgetAngle.setAutoFillBackground(true);
        
        /*
         * see eventFilter
         */
        _drawWidgetSingle.installEventFilter(this);
        _drawWidgetAll.installEventFilter(this);
        _drawWidgetAngle.installEventFilter(this);
        
        _widgetStack.addTab(&_drawWidgetSingle, "Single Projection");
        _widgetStack.addTab(&_drawWidgetAll, "Complete Projection");
        _widgetStack.addTab(&_drawWidgetAngle, "Angle Projection");
        _widgetStack.setCurrentIndex(0);
        _mainLayout.addWidget(&_widgetStack);
        QObject::connect(&_widgetStack, &QTabWidget::currentChanged, this,
                         &ResultWidget::tabChanged);
        this->setLayout(&_mainLayout);
    }

bool ResultWidget::eventFilter(QObject* watched, QEvent* event) {
        if(event->type() == QEvent::Paint && ( watched == &_drawWidgetSingle ||
            watched == &_drawWidgetAll || watched == &_drawWidgetAngle)){
            QWidget& widget = currWidget();
            QSize size = widget.size();
            if(_currProjection.empty()){
                return true;
            }
            std::size_t pixelY = _currProjection.size();
            std::size_t pixelX = _currProjection[0].size();
            
            std::size_t pixelWidth = std::min(size.width(), size.height())/ std::max(pixelX, pixelY);
            std::size_t pixelHeight = pixelWidth;
            
            float max = maxPixel(_currProjection);
            QPainter painter {&widget};
            QColor currColor {};
            
            for(int y = 0; y < pixelY; ++y){
                for (int x = 0; x < pixelX; ++x){
                    //There were some negative values, so to avoid bugs, currValue is at least zero
                    float currValue = std::max(_currProjection.at(y).at(x), 0.0f);
                    int colorVal = 255*currValue/max;
                    currColor.setRgb(colorVal, colorVal, colorVal);
                    painter.fillRect(x*pixelWidth, y*pixelHeight, pixelWidth, pixelHeight, currColor);
                }
            }
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
            case 2:
                return _drawWidgetAngle;
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
