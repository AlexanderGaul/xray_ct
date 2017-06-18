#pragma once

#include "AcquisitonPose"


class PoseViewer : public Widget
{
public:
	AcquisitionPose pose_;

	virtual void paintEvent(QPaintEvent* event);

};