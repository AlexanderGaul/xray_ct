#include "MPRWidget.h"

MPRWidget::MPRWidget(const VisualizationModel& visModel)
    : _visModel {visModel},
      _mprModel {}
{
}

void MPRWidget::paintEvent(QPaintEvent* p_e)
{
    QPainter painter(this);
    const Volume& vol = _visModel.volume();
    _mprModel.transferFunction().setRange(0, 0, vol.maxEntry());
    if(vol.getTotalVoxelCount() == 0)
    {
        painter.drawText(width()/2, height()/2, "Error: No data loaded!");
        return; // no data or invalid shape
    }
    int resolution = 50;
    int tileWidth = 20;
    painter.fillRect(0,0,resolution*tileWidth, resolution*tileWidth, QColor::fromRgb(0,0,0));

    float r = 0.f;

    Eigen::Vector3f normal(1,0,0);
    normal.normalize();

    Eigen::Vector3f intersect = normal * r;
    Eigen::Vector3f xDir(-normal(1), normal(0), 0);
    Eigen::Vector3f yDir = normal.cross(xDir);

    float x = calculateIntersectionLength(Line3f(intersect, xDir));
    float y = calculateIntersectionLength(Line3f(intersect, yDir));

    Eigen::Vector3f origin =
            calculateOrigin(calculateOrigin(intersect, xDir), yDir);

    float stepX = x/resolution;
    float stepY = y/resolution;

    for(int i = 0; i<resolution; ++i)
    {
        for(int j = 0; j<resolution; ++j)
        {
            Eigen::Vector3f curr = origin + stepX * i * xDir
                    + stepY * j * yDir;
            float intensity = vol.getVoxelLinearPhysical(curr);
            std::cout << intensity << std::endl;
            QColor color = _mprModel.transferFunction().classify(intensity);
            painter.fillRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth, color);
        }
    }

}

void MPRWidget::updateT4()
{
    Eigen::Vector3f t4 = _mprModel.t1() +
            (_mprModel.t2() - _mprModel.t1()) +
            (_mprModel.t3() - _mprModel.t1());
    _mprModel.setT4(Eigen::Vector3f(1,1,1));
}

void MPRWidget::setGranularity(int granularity)
{
    _mprModel.setGranularity(granularity*10);
    repaint();
}

void MPRWidget::setT1(Eigen::Vector3f position)
{
    _mprModel.setT1(position);
    repaint();
}

void MPRWidget::setT2(Eigen::Vector3f position)
{
    _mprModel.setT2(position);
    repaint();
}

void MPRWidget::setT3(Eigen::Vector3f position)
{
    _mprModel.setT3(position);
    repaint();
}


Eigen::Vector3f MPRWidget::t1() const
{
    return _mprModel.t1();
}

Eigen::Vector3f MPRWidget::t2() const
{
    return _mprModel.t2();
}

Eigen::Vector3f MPRWidget::t3() const
{
    return _mprModel.t3();
}

Eigen::Vector3f MPRWidget::t4() const
{
    return _mprModel.t4();
}

void MPRWidget::setColor(QColor color)
{
    _mprModel.setColor(color);
}

QColor MPRWidget::color() const
{
    return _mprModel.color();
}
