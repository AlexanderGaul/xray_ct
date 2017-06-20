//#include <algorithm>
#include "PoseDisplay.h"

void PoseDisplay::paintEvent(QPaintEvent* p_e)
{
	QPainter painter(this);
    QPen pen;

    // xc and yc are the center of the widget's rect.
    float x_center = width() * 0.5;
    float y_center = height() * 0.5;

    painter.setPen(Qt::black);

    // draw the cross lines.
    painter.drawLine(x_center, rect().top(), x_center, rect().bottom());
    painter.drawLine(rect().left(), y_center, rect().right(), y_center);

    if(pose_ == nullptr)
    {
    	return;
    }
    painter.resetTransform();
    painter.setPen(Qt::black);
    if(show_rays_){
        for(int i = 0; i < pose_->getPixelHorizontal(); i++)
        {
            Eigen::Vector3f pixel = pose_->getPixelCenter(i, 0);
            painter.drawLine(
                x_center + pose_->getSourcePosition()(0) * zoom_, 
                y_center - pose_->getSourcePosition()(1) * zoom_, 
                x_center + pixel(0) * zoom_, 
                y_center - pixel(1) * zoom_);
        }
    }
    
    painter.setBrush(Qt::white);

    //painter.translate(x_center, y_center);

    pen.setColor(Qt::red);
    pen.setWidth(2);
    
    painter.resetTransform();
    painter.translate(
        x_center + pose_->getSourcePosition()(0) * zoom_,
        y_center - pose_->getSourcePosition()(1) * zoom_
    );
    painter.setPen(pen);
    painter.drawEllipse(-2, -2, 4, 4);


    painter.resetTransform();
    pen.setColor(Qt::blue);
    painter.setPen(pen);

    //painter.setWidth(2)
    painter.translate(
        x_center + pose_->getDetectorCenter()(0) * zoom_, 
        y_center - pose_->getDetectorCenter()(1) * zoom_
       );
    painter.rotate(-pose_->getRotation() / M_PI * 180);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.
    float rx = 0;
    float ry = -pose_->getDetectorWidth() * zoom_ * 0.5;
    painter.drawRect(QRect(rx, ry, 4, pose_->getDetectorWidth() * zoom_));
  
}


void PoseDisplay::setShowRays(int state)
{
    if(state == 0)
    {
        show_rays_ = false;
    }
    else
    {
        show_rays_ = true;
    }
    update();
}
void PoseDisplay::setZoom(int zoom)
{
    zoom_ = zoom * 5;
    update();
}

void PoseDisplay::setPose(AcquisitionPose* pose)
{
    pose_ = pose;
    //update();
}


PoseDisplay::PoseDisplay() :
    show_rays_(false),
    zoom_(500),
    pose_(nullptr)
{}