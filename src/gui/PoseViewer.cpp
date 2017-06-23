#include "PoseViewer.h"

PoseViewer::PoseViewer(AcquisitionModel* model) 
    : _layout {}, _layoutBoxes {}, _zoomBox{}, _raysBox {}, _poseDisplay{model}, 
    _savePoseButton {"Store current pose"}, _deletePoseButton {"Delete current pose"}, 
    _clearButton {"Clear all poses"}, _model {model}
{

    _zoomBox.setMinimum(1);
    _zoomBox.setMaximum(1000);
    _zoomBox.setValue(100);

    _layout.addWidget(&_poseDisplay, 0, 0);
    _layoutBoxes.addWidget(&_raysBox, 0, 0);
    _layoutBoxes.addWidget(&_zoomBox, 0, 1);
    _layoutBoxes.setAlignment(Qt::AlignLeft);
    _layout.addLayout(&_layoutBoxes, 1, 0);
    
    _layoutButtons.addWidget(&_savePoseButton);
    _layoutButtons.addWidget(&_deletePoseButton);
    _layoutButtons.addWidget(&_clearButton);
    _layout.addLayout(&_layoutButtons, 2, 0);
    setLayout(&_layout);

    connect(&_savePoseButton, &QPushButton::pressed, _model, &AcquisitionModel::savePose);
    connect(&_deletePoseButton, &QPushButton::pressed, _model, &AcquisitionModel::deletePose);
    connect(&_clearButton, &QPushButton::pressed, _model, &AcquisitionModel::clearPoses);
    
    connect(&_savePoseButton, &QPushButton::pressed, this, &PoseViewer::sceneChanged);
    connect(&_deletePoseButton, &QPushButton::pressed, this, &PoseViewer::sceneChanged);
    connect(&_clearButton, &QPushButton::pressed, this, &PoseViewer::sceneChanged);
    
    connect(&_raysBox, SIGNAL(stateChanged(int)), &_poseDisplay, SLOT(setShowRays(int)));
    connect(&_zoomBox, SIGNAL(valueChanged(int)), &_poseDisplay, SLOT(setZoom(int)));

    connect(&_poseDisplay, SIGNAL(sceneChanged()), this, SLOT(sceneChanged()));
}
