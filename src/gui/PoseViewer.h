#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
// #include <QScrollArea>


#include "AcquisitionPose.h"
#include "PoseDisplay.h"


class PoseViewer : public QWidget
{
private:
    QGridLayout* layout_;
	QGridLayout* layout_boxes_;

	// QScrollArea* scroll_area_;
	QSpinBox* zoom_box_;
	QCheckBox* rays_box_;

	PoseDisplay* pose_display_;

	AcquisitionPose* pose_;

public:
	//void setPose(AcquisitionPose* pose);
	PoseViewer();
    PoseViewer(AcquisitionPose* pose);
    void setPose(AcquisitionPose* pose);

};