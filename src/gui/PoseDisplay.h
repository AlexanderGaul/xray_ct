#pragma once

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

#include "AcquisitionPose.h"

class PoseDisplay : public QWidget
{
	Q_OBJECT
private:
	bool show_rays_;
	int zoom_;
	//AcquisitionPose pose_;
	AcquisitionPose* pose_;
public:
	PoseDisplay();
	void setPose(AcquisitionPose* pose_);

	virtual void paintEvent(QPaintEvent* event);
	
	virtual void keyPressEvent(QKeyEvent* event);
	//virtual void mousePressEvent(QMouseEvent *event);	
public slots:
	void setShowRays(int state);
	void setZoom(int zoom);

};