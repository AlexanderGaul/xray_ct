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
        return;
    }
    int resolution = 50;
    int tileWidth = 20;
    painter.fillRect(0,0,resolution*tileWidth, resolution*tileWidth, QColor::fromRgb(0,0,0));

    float r = _mprModel.distance();

    Eigen::Vector3f normal = _mprModel.normal();
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

            QColor color = _mprModel.transferFunction().classify(intensity);
            painter.fillRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth, color);
        }
    }

}

void MPRWidget::setGranularity(int granularity)
{
    _mprModel.setGranularity(granularity*10);
    repaint();
}

void MPRWidget::setColor(QColor color)
{
    _mprModel.setColor(color);
}

QColor MPRWidget::color() const
{
    return _mprModel.color();
}
