#pragma once

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
// #include <QScrollArea>


#include "AcquisitionModel.h"
#include "PoseDisplay.h"


class PoseViewer : public QWidget
{
    Q_OBJECT
private:    
	// QScrollArea* scroll_area_;
	QSpinBox *_zoomBox;
	QCheckBox *_raysBox;
    
    /*
     * The number of positions for the automatic generation of Poses
     */
    QSpinBox *_poseCount1;
    QSpinBox *_poseCount2;
    QPushButton *_generatePosesButton;

	PoseDisplay *_poseDisplay;
    PoseDisplay *_poseDisplay2;
    PoseDisplay *_poseDisplay3;
    
    /*
     * Buttons to manipulate the pose stack in the AcquisitionModel
     */
    QPushButton *_savePoseButton;
    QPushButton *_deletePoseButton;
    QPushButton *_clearButton;

    AcquisitionModel& _model;
	bool _showRays;
	int _zoom;
    
public:
    
    PoseViewer(AcquisitionModel& model);

public slots:
    
    void generatePoses(){
        _model.addSphericalPoses(_poseCount1->value(), _poseCount2->value());
    }

};
