#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <memory>
#include <iostream>

#include <QColorDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

#include "DVRWidget.h"
#include "EDFhandler.h"
#include "MPRWidget.h"
#include "VisualizationModel.h"
#include "Volume.h"

/**
 * Widget to configure and execute visualization algorithms.
 *
 * Possible algorithms:
 *  2D-MPR (multiple planar reconstruction)
 *  3D-DVR (direct volume rendering)
 * @brief The VisualizationWidget class
 */
class VisualizationWidget : public QWidget
{
    Q_OBJECT
private:
    ///manages all data related to visualization
    VisualizationModel _visModel;

    ///all layout items for visulisation are composed in the main layout
    QGridLayout _mainLayout;
    ///manages all menu items
    QVBoxLayout _menuLayout;
    ///loads the Volume from a file
    QPushButton _loadFileButton;    
    ///loads the Volume from the ReconstructionPose
    QPushButton _loadRecButton;
    ///manages the color-choosing related widgets
    QHBoxLayout _colorLayout;
    ///displays the current color
    QLabel _colorLabel;
    ///opens a dialog to choose the color for visualization
    QPushButton _selectColorButton;
    ///choose the granularity of the 2D MPR
    QSlider _granularitySlider;
    ///manages configuration of DVR
    QVBoxLayout _dvrLayout;
    ///display description of DVR
    QLabel _dvrTitleLabel;
    ///manages label config of DVR
    QHBoxLayout _dvrAngleLayout;
    ///info text regarding angle for DVR
    QLabel _dvrAngleLabel;
    ///choose a suitable angle for DVR
    QSlider _dvrAngleSlider;
    ///display current angle (in degrees)
    QSpinBox _dvrAngleSpinBox;
    ///manages step width config of DVR
    QHBoxLayout _dvrStepWidthLayout;
    ///info text regarding angle for DVR
    QLabel _dvrStepWidthLabel;
    ///choose a suitable step width
    QSlider _dvrStepWidthSlider;
    ///display current step width
    QSpinBox _dvrStepWidthSpinBox;

    ///renders the 2D MPR visualization
    MPRWidget _mprWidget;
    ///paints the direct volume rendering (DVR)
    DVRWidget _dvrWidget;


    void updateColorLabel()
    {
        _colorLabel.setStyleSheet( "background-color: " + _visModel.color().name() );
    }

    void updateMPRWidget()
    {
        // zero based -> decrement it!
        int x = _visModel.volume().getNumVoxels()[0]-1;
        int y = _visModel.volume().getNumVoxels()[1]-1;
        int z = _visModel.volume().getNumVoxels()[2]-1;
        _mprWidget.setT1(Eigen::Vector3f(0,0,0));
        _mprWidget.setT2(Eigen::Vector3f(x,y,0));
        _mprWidget.setT3(Eigen::Vector3f(x,y,z));
    }

    /**
     * Reset the DRW widget to its default position and
     * calibrate the camera for a new volume.
     * @brief updateDRWWidget
     */
    void updateDVRWidget()
    {
        _dvrWidget.setAngle(0.0);
        _dvrWidget.calibrateCamera();
    }
    
public:
    
    VisualizationWidget() :
        _visModel {TransferFunction(TransferFunction::LinearPiece(0, 100, 0, 255, QColor::fromRgb(255,255,255)))},
        _mainLayout {},
        _menuLayout {},
        _loadFileButton {"Load from file"},
        _loadRecButton {"Load reconstruction"},
        _colorLayout {},
        _colorLabel {},
        _selectColorButton {"Select color"},
        _granularitySlider {},
        _dvrLayout {},
        _dvrTitleLabel {"DVR (direct volume rendering) configuration:"},
        _dvrAngleLayout {},
        _dvrAngleLabel {"Angle: "},
        _dvrAngleSlider {},
        _dvrAngleSpinBox {},
        _dvrStepWidthLayout {},
        _dvrStepWidthLabel {"Step width: "},
        _dvrStepWidthSlider {},
        _dvrStepWidthSpinBox {},
        _mprWidget {_visModel},
        _dvrWidget {_visModel}
    {
        updateColorLabel();
        _colorLayout.addWidget(&_colorLabel);
        _colorLayout.addWidget(&_selectColorButton);
        _menuLayout.addItem(&_colorLayout);

        _granularitySlider.setRange(1, 10);
        _granularitySlider.setOrientation(Qt::Horizontal);
        _granularitySlider.setMaximumWidth(200);
        _menuLayout.addWidget(&_granularitySlider);

        _dvrAngleLayout.addWidget(&_dvrAngleLabel);
        _dvrAngleSlider.setRange(1,360);
        _dvrAngleSlider.setOrientation(Qt::Horizontal);
        _dvrAngleLayout.addWidget(&_dvrAngleSlider);
        _dvrAngleSpinBox.setRange(1,360);
        _dvrAngleLayout.addWidget(&_dvrAngleSpinBox);
        _dvrAngleLabel.setMaximumWidth(200);
        _dvrAngleSlider.setMaximumWidth(200);
        _dvrAngleSpinBox.setMaximumWidth(200);

        _dvrStepWidthLayout.addWidget(&_dvrStepWidthLabel);
        _dvrStepWidthSlider.setRange(1,100);
        _dvrStepWidthSlider.setOrientation(Qt::Horizontal);
        _dvrStepWidthLayout.addWidget(&_dvrStepWidthSlider);
        _dvrStepWidthLayout.addWidget(&_dvrStepWidthSpinBox);
        _dvrStepWidthLabel.setMaximumWidth(200);
        _dvrStepWidthSlider.setMaximumWidth(200);
        _dvrStepWidthSpinBox.setMaximumWidth(200);

        _dvrLayout.addWidget(&_dvrTitleLabel);
        _dvrLayout.addItem(&_dvrAngleLayout);
        _dvrLayout.addItem(&_dvrStepWidthLayout);

        _menuLayout.addItem(&_dvrLayout);

        _menuLayout.addWidget(&_loadFileButton);
        _menuLayout.addWidget(&_loadRecButton);
        _mainLayout.addItem(&_menuLayout, 0, 0);

        _mainLayout.addWidget(&_mprWidget, 0, 1);
        _mainLayout.addWidget(&_dvrWidget, 0, 2);

        _mprWidget.setMinimumHeight(150);
        setLayout(&_mainLayout);

        connect(&_granularitySlider, &QSlider::valueChanged, &_mprWidget, &MPRWidget::setGranularity);
        connect(&_dvrAngleSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDVRAngle);
        // cast necessary because spinbox has also valueChanged(QString) signal
        connect(&_dvrAngleSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &VisualizationWidget::updateDVRAngle);
        connect(&_dvrStepWidthSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDVRStepWidth);
        connect(&_dvrStepWidthSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &VisualizationWidget::updateDVRStepWidth);
        connect(&_loadFileButton, &QPushButton::pressed, this, &VisualizationWidget::loadFromFile);
        connect(&_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);
        connect(&_selectColorButton, &QPushButton::pressed, this, &VisualizationWidget::changeColor);
    }
    
    void setRec(const std::shared_ptr<const Volume>& vol){
        _visModel.setVolume(*vol);
        updateMPRWidget();
        updateDVRWidget();
    }
    
signals:
    //emit this event if you want the volume from the requisition. It is set through a call of setRec
    void requestRecVolume();
    
public slots:
    void changeColor();

    /**
     * Loads a 3D medical image file into a Volume.
     *
     * After that, the visualization widgets are reset to the default viewpoints.
     * @brief loadFromFile
     */
    void loadFromFile(){
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("Medical image data (*.edf)"));
        QStringList fileNames;
        if (dialog.exec())
        {
            fileNames = dialog.selectedFiles();
            QString selection = fileNames.at(0);

            _visModel.setVolume(EDFHandler::read(selection.toStdString()));
            updateMPRWidget();
            updateDVRWidget();
        }
    }

    /**
     * Updates the angle for DVR.
     * The angle is given in degrees.
     * @brief updateDVRAngle
     * @param angle
     */
    void updateDVRAngle(int angle)
    {
        _dvrAngleSpinBox.setValue(angle);
        _dvrAngleSlider.setValue(angle);

        float radianAngle = (angle*M_PI)/180.0;
        _dvrWidget.setAngle(radianAngle);
    }

    /**
     * Updates the step width for DVR.
     * TODO: specify criteria for step width
     * CURRENTLY NOT USED BY DVR WIDGET!!!
     * @brief updateDVRStepWidth
     * @param stepWidth
     */
    void updateDVRStepWidth(int stepWidth)
    {
        _dvrStepWidthSpinBox.setValue(stepWidth);
        _dvrStepWidthSlider.setValue(stepWidth);

        //TODO: update value in dvr widget
    }
};

#endif // VISUALIZATIONWIDGET_H
