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
	QGridLayout _layoutBoxes;
    QHBoxLayout _layoutButtons;

	// QScrollArea* scroll_area_;
	QSpinBox _zoomBox;
	QCheckBox _raysBox;

	PoseDisplay _poseDisplay;
    
    /*
     * Buttons to manipulate the pose stack in the AcquisitionModel
     */
    QPushButton _savePoseButton;
    QPushButton _deletePoseButton;
    QPushButton _clearButton;

    AcquisitionModel *_model;
	bool _showRays;
	int _zoom;
signals:
    void sceneRotationChanged();

public:
    
    PoseViewer(AcquisitionModel* model);

public slots:
    void sceneChanged()
    {
        emit sceneRotationChanged();
    }

};
