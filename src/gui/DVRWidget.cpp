#include "DVRWidget.h"


DVRWidget::DVRWidget(VisualizationModel& visModel)
    : _visModel {visModel},
      _dvrModel {visModel.getDVRModel()}
{
    setFocusPolicy(Qt::ClickFocus);
    
    connect(&_dvrModel, &DVRModel::redraw, this, &DVRWidget::changedPose);
    
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
}

void DVRWidget::paintEvent(QPaintEvent*)
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
    
    
    float scale = std::min(static_cast<float>(width()) / _dvrModel.getCameraPose().getPixelHorizontal(), static_cast<float>(height()) / _dvrModel.getCameraPose().getPixelVertical());
    painter.scale(scale, scale);
    
    int count = vol.getBoundingBox().diagonal().norm() / _dvrModel.stepSize();
    
    for(int x = 0; x < _dvrModel.getCameraPose().getPixelHorizontal(); x++)
    {
        for(int y = 0; y < _dvrModel.getCameraPose().getPixelVertical(); y++)
        {
            
            float maxSample = 0;
            Eigen::ParametrizedLine<float, 3> ray;
            
            if(!_dvrModel.getPerspective())
            {
                ray = _dvrModel.getCameraPose().getRayOrthogonal(x, y);
            }
            else
            {
                ray = _dvrModel.getCameraPose().getRayPerspective(x, y);
            }

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
                painter.fillRect(x, y, 1, 1, color);
            }
    
        }
    }

}

void DVRWidget::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Left) {
        _dvrModel.getCameraPose().addRotationZ(-0.05f);
        emit _dvrModel.updateRotationZ();
        update();
    } else if(event->key() == Qt::Key_Right) {  
        _dvrModel.getCameraPose().addRotationZ(0.05f);
        emit _dvrModel.updateRotationZ();
        update();
    } else if(event->key() == Qt::Key_Up) {
        _dvrModel.getCameraPose().addRotationY(0.05f);
        emit _dvrModel.updateRotationY();
        update();
    } else if(event->key() == Qt::Key_Down) {
        _dvrModel.getCameraPose().addRotationY(-0.05f);

        emit _dvrModel.updateRotationY();
        update();
    }
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

void DVRWidget::updateVolume(Volume vol)
{
    setColorRange(0, vol.maxEntry());
    
}

void DVRWidget::changedPose()
{
    update();
}


int DVRWidget::heightForWidth(int w) const
{
    float aspectRatio = _dvrModel.getCameraPose().getPixelVertical() / static_cast<float>(_dvrModel.getCameraPose().getPixelHorizontal());
    return static_cast<int>(aspectRatio * w);
}

QSize DVRWidget::sizeHint() const
{
    int heightHint = heightForWidth(width());

    return QSize(QWidget::sizeHint().width(), heightHint);
}
bool DVRWidget::hasHeightForWidth() const
{
    return true;
}

void DVRWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    updateGeometry();
}


