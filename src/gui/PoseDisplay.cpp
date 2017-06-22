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

    if(_model == nullptr)
    {
    	return;
    }
    painter.resetTransform();
    painter.setPen(Qt::black);
    const Eigen::Vector2i detectorSize = _model->getDetectorSize();
    const Eigen::Vector3f sourcePos = _model->getSourcePosition();
    
    if(_showRays){
        for(int i = 0; i < detectorSize.x(); i++)
        {
            for(int j = 0; j < detectorSize.y(); j++)
            {
                Eigen::Vector3f pixel = _model->getPixelCenter(i, j);
                painter.drawLine(
                    x_center + sourcePos.x() * _zoom, 
                    y_center - sourcePos.y() * _zoom, 
                    x_center + pixel(0) * _zoom, 
                    y_center - pixel(1) * _zoom);
            }
        }
    }
    
    painter.setBrush(Qt::white);

    //painter.translate(x_center, y_center);

    pen.setColor(Qt::red);
    pen.setWidth(2);
    
    painter.resetTransform();
    painter.translate(
        x_center + sourcePos.x() * _zoom,
        y_center - sourcePos.y() * _zoom
    );
    painter.setPen(pen);
    painter.drawEllipse(-2, -2, 4, 4);




    painter.resetTransform();
    pen.setColor(Qt::blue);
    painter.setPen(pen);

    //painter.scale(zoom_, zoom_);
    
    std::array<Eigen::Vector3f, 4> detCorners = _model->getDetector();
    
    for(int i = 0; i < 4; i++)
    {
        painter.drawLine(
            x_center + detCorners[i].x() * _zoom,
            y_center - detCorners[i].y() * _zoom,
            x_center + detCorners[(i + 1) % 4].x() * _zoom,
            y_center - detCorners[(i + 1) % 4].y() * _zoom  
        );
    }
    
    
    /*
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
    */
}



void PoseDisplay::setShowRays(int state)
{
    if(state == 0)
    {
        _showRays = false;
    }
    else
    {
        _showRays = true;
    }
    update();
}
void PoseDisplay::setZoom(int zoom)
{
    _zoom = zoom * 5;
    update();
}

PoseDisplay::PoseDisplay(AcquisitionModel *model) :
    _showRays (false),
    _zoom (500),
    _model (model)
{
    setFocusPolicy(Qt::ClickFocus);
    //grabKeyboard();
}
/*
void PoseDisplay::mousePressEvent(QMouseEvent* event)
{
    grabKeyboard();
}*/

void PoseDisplay::keyPressEvent(QKeyEvent* event)
{
    if(_model != nullptr)
    {       
        if(event->key() == Qt::Key_Left)
        {
            _model->updateRotation(RotationAxis::Z, 0.1f);
            update();
        }
        else if(event->key() == Qt::Key_Right)
        {
            _model->updateRotation(RotationAxis::Z, -0.1f);
            update();
        }
        else if(event->key() == Qt::Key_Up)
        {
            _model->updateRotation(RotationAxis::X, 0.1f);
            update();
        }
        else if(event->key() == Qt::Key_Down)
        {
            _model->updateRotation(RotationAxis::X, -0.1f);
            update();
        }
    }
}
