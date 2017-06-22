#pragma once

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

#include "AcquisitionModel.h"

/*
 * Only paints the current aquisition pose
 */
class PoseDisplay : public QWidget
{
	Q_OBJECT
private:
	bool _showRays;
	int _zoom;
	AcquisitionModel *_model;
public:
	PoseDisplay(AcquisitionModel *model);

	virtual void paintEvent(QPaintEvent *event);
	
	virtual void keyPressEvent(QKeyEvent *event);
public slots:
	void setShowRays(int state);
	void setZoom(int zoom);

};
