#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
// #include <QScrollArea>


#include "AcquisitionModel.h"
#include "PoseDisplay.h"


class PoseViewer : public QWidget
{
private:
    QGridLayout _layout;
	QGridLayout _layoutBoxes;

	// QScrollArea* scroll_area_;
	QSpinBox _zoomBox;
	QCheckBox _raysBox;

	PoseDisplay _poseDisplay;

	bool _showRays;
	int _zoom;

public:
    
    PoseViewer(AcquisitionModel* pose);

};
