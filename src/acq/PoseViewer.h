#pragma once

#include <QWidget>
#include <QPainter>


#include "AcquisitionPose.h"


class PoseViewer : public QWidget
{
	Q_OBJECT
private:
    bool show_rays_;
    float zoom_ = 1000;

public:

    AcquisitionPose pose_;

    PoseViewer();

    virtual void paintEvent(QPaintEvent* event);

public slots:
	void setShowRays(int state);
	void setZoom(int zoom);
};