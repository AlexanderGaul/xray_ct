#include "PoseDisplay.h"

void PoseDisplay::paintEvent(QPaintEvent* p_e)
{
	QPainter painter(this);
    QPen pen;
    
    int xAxis;
    int yAxis;
    
    if(_axis == 2)
    {
        xAxis = 0;
        yAxis = 1;
    }
    else if(_axis == 0)
    {
        xAxis = 1;
        yAxis = 2;
    }
    else if(_axis == 1)
    {
        xAxis = 0;
        yAxis = 2;
    }

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
    
    
    for(int i = 0; i < detectorSize.x(); i += detectorSize.x() - 1)
    {
        for(int j = 0; j < detectorSize.x(); j += detectorSize.y() - 1)
        {
            Eigen::Vector3f pixel = _model->getPixelCenter(i, j);
                painter.drawLine(
                    x_center + sourcePos(xAxis) * _zoom,
                    y_center - sourcePos(yAxis) * _zoom,
                    x_center + pixel(xAxis) * _zoom,
                    y_center - pixel(yAxis) * _zoom);
        }
    }
    
    
    if(_showRays)
    {
        for(int i = 0; i < detectorSize.x(); i++)
        {
            for(int j = 0; j < detectorSize.y(); j++)
            {
                Eigen::Vector3f pixel = _model->getPixelCenter(i, j);
                painter.drawLine(
                    x_center + sourcePos(xAxis) * _zoom,
                    y_center - sourcePos(yAxis) * _zoom,
                    x_center + pixel(xAxis) * _zoom,
                    y_center - pixel(yAxis) * _zoom);
            }
        }
    }
    
    //painter.setBrush(Qt::white);

    //painter.translate(x_center, y_center);

    pen.setColor(Qt::red);
    pen.setWidth(2);
    
    painter.resetTransform();
    painter.translate(
        x_center + sourcePos(xAxis) * _zoom,
        y_center - sourcePos(yAxis) * _zoom
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
            x_center + detCorners[i](xAxis) * _zoom,
            y_center - detCorners[i](yAxis) * _zoom,
            x_center + detCorners[(i + 1) % 4](xAxis) * _zoom,
            y_center - detCorners[(i + 1) % 4](yAxis) * _zoom
        );
    }
    
    
    //paint the bounding box of the main volume
    auto boundingBox = _model->getBoundingBox();
    auto topLeftCorner = boundingBox.corner(Eigen::AlignedBox3f::TopLeftFloor);
    auto lowerRightCorner = boundingBox.corner(Eigen::AlignedBox3f::BottomRightFloor);
    
    auto minCorner = boundingBox.min();
    auto maxCorner = boundingBox.max();
    
    
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.resetTransform();
    painter.translate(x_center, y_center);
    painter.drawRect( minCorner(xAxis) *_zoom, minCorner(yAxis) *_zoom,
                      (maxCorner(xAxis) - minCorner(xAxis)) *_zoom,
                      (maxCorner(yAxis) - minCorner(yAxis)) *_zoom);
                     
    
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
    PoseDisplay(model, 2)
{
    setFocusPolicy(Qt::ClickFocus);
    //grabKeyboard();
}
PoseDisplay::PoseDisplay(AcquisitionModel *model, int axis) :
    _showRays (false),
    _zoom (500),
    _model (model),
    _axis(axis)
{
    
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
            _model->updateRotation(RotationAxis::Z, -0.1f);
            //emit sceneChanged();
            emit _model->poseChanged();
            //update();
        }
        else if(event->key() == Qt::Key_Right)
        {
            _model->updateRotation(RotationAxis::Z, 0.1f);
            //emit sceneChanged();
            emit _model->poseChanged();
            //update();
        }
        else if(event->key() == Qt::Key_Up)
        {
            _model->updateRotation(RotationAxis::X, 0.1f);
            //emit sceneChanged();
            emit _model->poseChanged();
            //update();
        }
        else if(event->key() == Qt::Key_Down)
        {
            _model->updateRotation(RotationAxis::X, -0.1f);
            //emit sceneChanged();
            emit _model->poseChanged();
            //update();
        }
    }
}
