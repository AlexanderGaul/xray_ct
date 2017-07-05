#pragma once

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
// #include <QScrollArea>


#include "AcquisitionModel.h"
#include "PoseDisplay.h"


class PoseViewer : public QWidget
{
    Q_OBJECT
private:
    QGridLayout _layout;
    QGridLayout _viewsLayout;
	QGridLayout _layoutBoxes;
    QHBoxLayout _layoutButtons;

	// QScrollArea* scroll_area_;
	QSpinBox _zoomBox;
	QCheckBox _raysBox;

	PoseDisplay _poseDisplay;
    PoseDisplay _poseDisplay2;
    PoseDisplay _poseDisplay3;
    
    /*
     * Buttons to manipulate the pose stack in the AcquisitionModel
     */
    QPushButton _savePoseButton;
    QPushButton _deletePoseButton;
    QPushButton _clearButton;
    
    QPushButton _sphericalPosesButton;

    AcquisitionModel *_model;
	bool _showRays;
	int _zoom;
    
public:
    
    PoseViewer(AcquisitionModel* model);

public slots:
    void sceneChanged()
    {
        emit _model->poseChanged();
    }

};
