#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>

#include "PoseViewer.h"
#include "AcquisitionPose.h"
class PoseWidget : public QWidget
{
	Q_OBJECT
private:
	QGridLayout* layout_;
	QGridLayout* layout_boxes_;

	QScrollArea* scroll_area_;
	PoseViewer* pose_viewer_;
	QSpinBox* zoom_box_;
	QCheckBox* rays_box_;

	AcquisitionPose default_pose_;
public:
	PoseWidget();

};