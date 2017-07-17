#include "MPRWidget.h"

MPRWidget::MPRWidget(const VisualizationModel& visModel)
    : _visModel {visModel},
      _mprModel {}
{
}

void MPRWidget::paintEvent(QPaintEvent* p_e)
{
    QPainter painter(this);
    if(_visModel.volume().getTotalVoxelCount() == 0)
    {
        painter.drawText(width()/2, height()/2, "Error: No data loaded!");
        return; // no data or invalid shape
    }
    if(!_visModel.volume().validPosition(_mprModel.t1()) ||
            !_visModel.volume().validPosition(_mprModel.t2()) ||
            !_visModel.volume().validPosition(_mprModel.t3()) ||
            !_visModel.volume().validPosition(_mprModel.t4()))
    {
        painter.drawText(width()/2, height()/2, "Error: Invalid plane configured!");
        return;
    }
    // for a good rendering, sample each real pixel of the cut plane with 10*10*10 voxels
    int sampling = 10;
    int steps = _visModel.volume().getNumVoxels()[0]*sampling;
    // set background
    painter.fillRect(0,0, width(), height(), Qt::black);

    int tileWidth = std::min(width(), height());
    tileWidth /= steps;

    Eigen::Vector3f d1 = _mprModel.t2() - _mprModel.t1();
    d1 /= (steps);
    Eigen::Vector3f d2 = _mprModel.t3() - _mprModel.t2();
    d2 /= (steps);

    for(int i = 0; i<steps; ++i)
    {
        Eigen::Vector3f curr = _mprModel.t1() + d2 * i;
        for(int j = 0; j<steps; ++j)
        {
            float intensity = _visModel.volume().getVoxelLinear(curr);
            if(intensity < 0) intensity = 0;
            QColor color = _visModel.transferFunction().classify(intensity);

            painter.fillRect(QRect(i*tileWidth, j*tileWidth, tileWidth, tileWidth), color);
            curr += d1;
        }
    }
}

void MPRWidget::updateT4()
{
    Eigen::Vector3f t4 = _mprModel.t1() +
            (_mprModel.t2() - _mprModel.t1()) +
            (_mprModel.t3() - _mprModel.t1());
    _mprModel.setT4(t4);
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
