#include "PoseViewer.h"

PoseViewer::PoseViewer(AcquisitionModel& model) : 
    _layout {}, 
    _viewsLayout {},
    _layoutBoxes {}, 
    _generatorLayout {},
    _zoomBox{}, 
    _raysBox {}, 
    _poseCount1 {},
    _poseCount2 {},
    _generatePosesButton {"Generate Poses"},
    _poseDisplay{model},
    _poseDisplay2{model, 0},
    _poseDisplay3{model, 1},
    _savePoseButton {"Store current pose"}, 
    _deletePoseButton {"Delete current pose"}, 
    _clearButton {"Clear all poses"},
    _sphericalPosesButton {"Add Spherical Set of Poses"},
     _model {model}
{

    _zoomBox.setMinimum(1);
    _zoomBox.setMaximum(1000);
    _zoomBox.setValue(100);

    _viewsLayout.addWidget(&_poseDisplay, 0, 0);
    _viewsLayout.addWidget(&_poseDisplay2, 1, 1);
    _viewsLayout.addWidget(&_poseDisplay3, 1, 0);
    
    _layoutBoxes.addWidget(&_raysBox, 0, 0);
    _layoutBoxes.addWidget(&_zoomBox, 0, 1);
    _layoutBoxes.setAlignment(Qt::AlignLeft);
    
    _layout.addLayout(&_viewsLayout, 0, 0);
    _layout.addLayout(&_layoutBoxes, 1, 0);
    
    
    _layoutButtons.addWidget(&_savePoseButton);
    _layoutButtons.addWidget(&_deletePoseButton);
    _layoutButtons.addWidget(&_clearButton);
    _layoutButtons.addWidget(&_sphericalPosesButton);
    _layout.addLayout(&_layoutButtons, 3, 0);
    
    _generatorLayout.addWidget(&_poseCount1);
    _generatorLayout.addWidget(&_poseCount2);
    _generatorLayout.addWidget(&_generatePosesButton);
    
    _poseCount1.setMinimum(1);
    _poseCount2.setMinimum(1);
    _layout.addLayout(&_generatorLayout, 4, 0);
    
    setLayout(&_layout);
    
    connect(&_generatePosesButton, &QPushButton::pressed, this, &PoseViewer::generatePoses);
    
    connect(&_savePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::savePose);
    connect(&_deletePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::deletePose);
    connect(&_clearButton, &QPushButton::pressed, &_model, &AcquisitionModel::clearPoses);
    connect(&_sphericalPosesButton, &QPushButton::pressed, &_model, &AcquisitionModel::addHalfSphericalPoses);
    
    connect(&_savePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::savePose);
    connect(&_deletePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::deletePose);
    connect(&_clearButton, &QPushButton::pressed, &_model, &AcquisitionModel::clearPoses);

    
    connect(&_raysBox, SIGNAL(stateChanged(int)), &_poseDisplay, SLOT(setShowRays(int)));
    connect(&_zoomBox, SIGNAL(valueChanged(int)), &_poseDisplay, SLOT(setZoom(int)));
    
    connect(&_raysBox, SIGNAL(stateChanged(int)), &_poseDisplay2, SLOT(setShowRays(int)));
    connect(&_zoomBox, SIGNAL(valueChanged(int)), &_poseDisplay2, SLOT(setZoom(int)));
    
    connect(&_raysBox, SIGNAL(stateChanged(int)), &_poseDisplay3, SLOT(setShowRays(int)));
    connect(&_zoomBox, SIGNAL(valueChanged(int)), &_poseDisplay3, SLOT(setZoom(int)));

   //connect(&_poseDisplay, &PoseDisplay::sceneChanged(), this, PoseView::sceneChanged()));
}
