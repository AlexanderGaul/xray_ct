

#include <iostream>
#include "PoseViewer.h"

PoseViewer::PoseViewer(AcquisitionPose* pose) :
    PoseViewer()
{
    pose_ = pose;
    pose_display_->setPose(pose);
}

PoseViewer::PoseViewer()
{
    pose_display_ = new PoseDisplay();

    zoom_box_ = new QSpinBox();
    zoom_box_->setMinimum(1);
    zoom_box_->setMaximum(1000);
    zoom_box_->setValue(100);

    rays_box_ = new QCheckBox();

    layout_ = new QGridLayout();
    layout_boxes_ = new QGridLayout();

    layout_->addWidget(pose_display_, 0, 0);
    layout_boxes_->addWidget(rays_box_, 0, 0);
    layout_boxes_->addWidget(zoom_box_, 0, 1);
    layout_boxes_->setAlignment(Qt::AlignLeft);
    layout_->addLayout(layout_boxes_, 1, 0);

    setLayout(layout_);

    connect(rays_box_, SIGNAL(stateChanged(int)), pose_display_, SLOT(setShowRays(int)));
    connect(zoom_box_, SIGNAL(valueChanged(int)), pose_display_, SLOT(setZoom(int)));
}

void PoseViewer::setPose(AcquisitionPose* pose)
{
    pose_display_->setPose(pose);
    pose_ = pose;
}

