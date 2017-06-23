#include "PoseViewer.h"

PoseViewer::PoseViewer(AcquisitionModel* model) 
    : _layout {}, _layoutBoxes {}, _zoomBox{}, _raysBox {}, _poseDisplay{model}
{

    _zoomBox.setMinimum(1);
    _zoomBox.setMaximum(1000);
    _zoomBox.setValue(100);

    _layout.addWidget(&_poseDisplay, 0, 0);
    _layoutBoxes.addWidget(&_raysBox, 0, 0);
    _layoutBoxes.addWidget(&_zoomBox, 0, 1);
    _layoutBoxes.setAlignment(Qt::AlignLeft);
    _layout.addLayout(&_layoutBoxes, 1, 0);

    setLayout(&_layout);

    connect(&_raysBox, SIGNAL(stateChanged(int)), &_poseDisplay, SLOT(setShowRays(int)));
    connect(&_zoomBox, SIGNAL(valueChanged(int)), &_poseDisplay, SLOT(setZoom(int)));

    connect(&_poseDisplay, SIGNAL(sceneChanged()), this, SLOT(sceneChanged()));
}
