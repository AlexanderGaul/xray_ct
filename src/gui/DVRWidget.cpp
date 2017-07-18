#include "DVRWidget.h"

Eigen::Vector3f DVRWidget::calculateCameraPosition(const Volume& volume)
{
    if(volume.getTotalVoxelCount() == 0)
    {
        _mode = EMPTY;
        return Eigen::Vector3f(0,0,0); // volume was empty
    }
    else
    {
        _mode = FILLED;
    }

    Eigen::Vector3f direction(0,0,0);
    direction(0) += std::cos(_dvrModel.angle());
    direction(1) += std::sin(_dvrModel.angle());
    float size = volume.getNumVoxels()[0] *
                    volume.getSpacing()[0];
    for(int i = 0; i<3; ++i)
    {
        direction(i) *= 2*size;
    }

    // center is always centered at the origin, so just consider the direction
    return direction;
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
      _dvrModel {M_PI, 5, calculateCameraPosition(_visModel.volume()), 0.5,
                 TransferFunction(LinearPiece(0, 100, 0, 255, QColor::fromRgb(255,255,255)))}
{
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
    float stepSize = _dvrModel.stepSize();
    if(stepSize == 0)
    {        
        painter.drawText(width()/2, height()/2, "Error: DVR not possible with step size = 0!");
        return;
    }
    stepSize /= 100;
    int steps = sizePixelX/stepSize;

    float reso = std::min(sizePixelX, sizePixelZ) / stepSize;

    int tileWidth = std::min(width(), height()) / reso;

    // calculate position of pixel most down and left in camera
    Eigen::Vector3f better = _dvrModel.position();
    better(0) -= std::sin(angle)*sizePixelX*0.5*reso;
    better(1) -= std::cos(angle)*sizePixelX*0.5*reso;
    better(2) = box.corner(Eigen::AlignedBox3f::BottomLeftFloor)(2);

    std::cout << "##########" << std::endl;

    for(int i = 0; i<=reso; ++i)
    {
        Eigen::Vector3f tmp = better + i * sizePixelZ * Eigen::Vector3f(0, 0, std::cos(angle));
        for(int j = 0; j<=reso; ++j)
        {
            Line3f ray(tmp, correction);

            // intersection between current pixel and volume
            Eigen::Vector3f intersect = RayTracing::boxIntersect(box, ray);
            std::cout << tmp(0) << ","<<tmp(1)<<","<<tmp(2)<<std::endl;

            // do MIP (maximum intensity projection)
            float max = 0.0;
            // NaN check (NaN are odd in comparison and will not execute inner block)
            if(intersect(0) == intersect(0))
            {
                for(int i = 0; i<steps; ++i)
                {
                    // calculate intensity using trilinear interpolation
                    float intensity = vol.getVoxelLinearPhysical(intersect);
                    if(intensity > max)
                    {
                        max = intensity;
                    }
                    intersect += (stepSize) * correction;
                }
                std::cout << max << std::endl;
            }
            // paint measured volume
            QColor color = _dvrModel.transferFunction().classify(max);
            painter.fillRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth, color);
            // update pixel position
            tmp += sizePixelX * Eigen::Vector3f(-std::sin(angle), std::cos(angle), 0);
        }
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
