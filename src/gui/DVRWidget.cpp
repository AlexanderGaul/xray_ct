#include "DVRWidget.h"

Eigen::Vector3f DVRWidget::calculateCameraPosition(const Volume& volume)
{
    return _dvrCamera.calculateCameraPosition(volume, _dvrModel.angle());
}

Eigen::Vector3f DVRWidget::normalize(Eigen::Vector3f x)
{
    float sum = 0.0;
    for(int i = 0; i<x.rows(); ++i)
    {
        sum += std::abs(x(i));
    }
    if(sum > 0)
    {
        for(int i = 0; i<x.rows(); ++i)
        {
            x(i) /= sum;
        }
    }
    return x;
}

DVRWidget::DVRWidget(const VisualizationModel& visModel)
    : _visModel {visModel},
      _dvrModel {M_PI, 5, calculateCameraPosition(_visModel.volume()), 0.0025,
                 TransferFunction(LinearPiece(0, 100, 0, 255, QColor::fromRgb(255,255,255)))},
      _dvrCamera {},
      
      _pose {0.2f, 0.8f, 0.8f, 200, 200}
{
    setFocusPolicy(Qt::ClickFocus);
}

void DVRWidget::paintEvent(QPaintEvent* p_e)
{
    
    QPainter painter(this);
    if(_visModel.volume().getTotalVoxelCount() == 0)
    {
        painter.drawText(width()/2, height()/2, "Error: No data loaded!");
        return;
    }
    painter.fillRect(0, 0, width(), height(), QColor::fromRgb(0,0,0));

    const Volume& vol = _visModel.volume();
    Eigen::AlignedBox3f box = vol.getBoundingBox();

    float angle = _dvrModel.angle();
    int resolution = _dvrModel.resolution();

    // get direction vector between volume center and detector center
    Eigen::Vector3f direction(std::cos(angle),std::sin(angle),0);
    // normalize direction
    direction = normalize(direction);
    // compute direction vector between detector and volume
    Eigen::Vector3f correction = direction;
    // small negative values cause numerical instability...
    for(int i = 0; i<3; ++i)
    {
        if(std::abs(correction(i)) > 0.0001)
        {
            correction(i) = -correction(i);
        }
    }
    // get physical size of one pixel
    float sizePixelX = vol.getNumVoxels()[0] *
                    vol.getSpacing()[0] / resolution;
    float sizePixelZ = vol.getNumVoxels()[2] *
                    vol.getSpacing()[2] / resolution;
    float stepSizeLength = _dvrModel.stepSize();
    if(stepSizeLength == 0)
    {        
        painter.drawText(width()/2, height()/2, "Error: DVR not possible with step size = 0!");
        return;
    }
    stepSizeLength /= 100;

    float reso = std::min(sizePixelX, sizePixelZ) / stepSizeLength;

    int tileWidth = std::min(width(), height()) / reso;

    // calculate position of pixel most down and left in camera
    Eigen::Vector3f better = _dvrModel.position();
    better(0) -= std::sin(angle)*sizePixelX*0.5*reso;
    better(1) -= std::cos(angle)*sizePixelX*0.5*reso;
    better(2) = box.corner(Eigen::AlignedBox3f::BottomLeftFloor)(2);
    
    /*
    MIP mip;
    for(int i = 0; i<=reso; ++i)
    {
        Eigen::Vector3f tmp = better + i * sizePixelZ * Eigen::Vector3f(0, 0, std::cos(angle));
        for(int j = 0; j<=reso; ++j)
        {
            float mipValue = mip.calculateMIP(
                        vol,
                        tmp,
                        correction,
                        stepSizeLength);
            // paint measured volume
            QColor color = _dvrModel.transferFunction().classify(mipValue);
            painter.fillRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth, color);
            // update pixel position
            tmp += sizePixelX * Eigen::Vector3f(-std::sin(angle), std::cos(angle), 0);
        }
    }
    */
    tileWidth = std::min(width(), height()) / _pose.getPixelHorizontal();
    int count = vol.getBoundingBox().diagonal().norm() / _dvrModel.stepSize();
    
    for(int x = 0; x < _pose.getPixelHorizontal(); x++)
    {
        for(int y = 0; y < _pose.getPixelVertical(); y++)
        {
            float maxSample = 0;
            Eigen::ParametrizedLine<float, 3> ray = _pose.getRay(x, y);
            float distance = RayTracing::boxIntersectHelper(vol.getBoundingBox(), ray);
            
            if(!std::isnan(distance))
            {
                for(int i = 0; i <= count; i++)
                {
                    float sample = vol.getVoxelLinearPhysical(ray.pointAt(distance));
                    if(sample > maxSample)
                    {
                        maxSample = sample;
                    }
                    distance += _dvrModel.stepSize();
                }
                QColor color = _dvrModel.transferFunction().classify(maxSample);
                painter.fillRect(x * tileWidth, y * tileWidth, tileWidth, tileWidth, color);
            }
        }
    }
}

void DVRWidget::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Left) {
        _pose.addRotationGlobalZ(0.05f);
        //emit sceneChanged();
        //emit _model.poseChanged();
        update();
    } else if(event->key() == Qt::Key_Right) {  
        _pose.addRotationGlobalZ(-0.05f);
        //emit sceneChanged();
        //emit _model.poseChanged();
        update();
    } else if(event->key() == Qt::Key_Up) {
        _pose.addRotationLocalY(-0.05f);
        //emit sceneChanged();
        //emit _model.poseChanged();
        update();
    } else if(event->key() == Qt::Key_Down) {
        _pose.addRotationLocalY(0.05f);
        //emit sceneChanged();
        //emit _model.poseChanged();
        update();
    }
}


void DVRWidget::setAngle(float angle)
{
    if(angle != _dvrModel.angle())
    {
        _dvrModel.setAngle(angle);
        calibrateCamera();
        repaint();
    }
}

void DVRWidget::calibrateCamera()
{
    _dvrModel.setPosition(calculateCameraPosition(_visModel.volume()));
}

void DVRWidget::setColor(QColor color)
{
    _dvrModel.setColor(color);
}

QColor DVRWidget::color() const
{
    return _dvrModel.color();
}

void DVRWidget::setStepSize(float stepSize)
{
    _dvrModel.setStepSize(stepSize);
    repaint();
}

void DVRWidget::setColorRange(float from, float to)
{
    _dvrModel.setColorRange(from, to);
}
