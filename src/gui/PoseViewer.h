#pragma once

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
// #include <QScrollArea>


#include "AcquisitionModel.h"
#include "PoseDisplay.h"


class PoseViewer : public QWidget
{
    Q_OBJECT
private:
    QGridLayout _layout;
	QGridLayout _layoutBoxes;

	// QScrollArea* scroll_area_;
	QSpinBox _zoomBox;
	QCheckBox _raysBox;

	PoseDisplay _poseDisplay;

	bool _showRays;
	int _zoom;

signals:
    void sceneRotationChanged();

public:
    
    PoseViewer(AcquisitionModel* pose);

public slots:
    void sceneChanged()
    {
        emit sceneRotationChanged();
    }

};
