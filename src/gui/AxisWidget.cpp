#include "AxisWidget.h"

void AxisWidget::paintEvent(QPaintEvent*) {
    const int minSidelength = std::min(this->height(), this->width());
    const float vecLength = minSidelength / 2.f;
    //Since the given Rotation is the rotation of the camera, the inverse of the
    //rotation is the rotation of the object
    auto rot = _model.getRotation().inverse();
    std::array<Eigen::Vector3f, 3> axesVecs {
        -rot * Eigen::Vector3f::UnitX() * vecLength * 0.8f,
        -rot * Eigen::Vector3f::UnitY() * vecLength * 0.8f, 
        -rot * Eigen::Vector3f::UnitZ() * vecLength * 0.8f};
            
    std::array<int, 3> order {0, 1, 2};
        
    //hardcoded bubblesort for three elements
    //ensures that if two axes overlap the right one is painted in front of the other one
    if(axesVecs.at(0).x() > axesVecs.at(1).x()){
        std::swap(axesVecs.at(0), axesVecs.at(1));
        std::swap(order.at(0), order.at(1));
    }
    if(axesVecs.at(1).x() > axesVecs.at(2).x()){
        std::swap(axesVecs.at(1), axesVecs.at(2));
        std::swap(order.at(1), order.at(2));
    }
    if(axesVecs.at(0).x() > axesVecs.at(1).x()){
        std::swap(axesVecs.at(0), axesVecs.at(1));
        std::swap(order.at(0), order.at(1));
    }
        
    std::array<QString, 3> labels {"X", "Y", "Z"};
    std::array<QPen, 3> colors {QPen{QColor{Qt::red}}, QPen{QColor{Qt::green}}, QPen{QColor{Qt::blue}}};
        
    QPainter painter {this};
    painter.translate(this->width()/2, this->height()/2);
    painter.setPen(colors.at(order.at(0)));
    painter.drawLine(0, 0, axesVecs[0].y(), axesVecs[0].z());
    painter.drawText(axesVecs[0].y()*1.1, axesVecs[0].z()*1.1, labels.at(order.at(0)));
        
    painter.setPen(colors.at(order.at(1)));
    painter.drawLine(0, 0, axesVecs[1].y(), axesVecs[1].z());
    painter.drawText(axesVecs[1].y()*1.1, axesVecs[1].z()*1.1, labels.at(order.at(1)));
        
    painter.setPen(colors.at(order.at(2)));
    painter.drawLine(0, 0, axesVecs[2].y(), axesVecs[2].z());
    painter.drawText(axesVecs[2].y()*1.1, axesVecs[2].z()*1.1, labels.at(order.at(2)));
}
