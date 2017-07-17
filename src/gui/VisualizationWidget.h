#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <iostream>
#include <memory>
#include <vector>

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
    ///manages the MPR configuration
    QVBoxLayout _mprLayout;
    ///displays title of MPR
    QLabel _mprTitleLabel;
    ///shows the dimensions of the volume on screen
    QLabel _volumeInfoLabel;
    ///layout for entering coordinates
    QGridLayout _coordinateLayout;
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

    void updateVolumeInfo()
    {
        Eigen::Vector3i size = _visModel.volume().getNumVoxels();
        int x = size[0], y = size[1], z = size[2];
        _volumeInfoLabel.setText("Volume size: ("+QString::number(x)+","+QString::number(y)+","+QString::number(z)+")");
    }

    void setLimit(int x, int y, int limit)
    {
        static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(x, y)->widget())->setRange(0,limit-1);
    }

    void updateCoordinateLimits()
    {
        Eigen::Vector3i size = _visModel.volume().getNumVoxels();
        for(int i = 0; i<3; ++i)
        {
            for(int j = 0; j<3; ++j)
            {
                setLimit((j+1),(i+1),size[i]);
            }
        }
    }

    void updateVolumeChanged()
    {
        updateDVRWidget();
        updateVolumeInfo();
        updateCoordinateLimits();
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

    /**
     * Calculate the fourth edge point of the plane used
     * in multiple planar reconstruction (MPR).
     */
    void updateT4()
    {        
        _mprWidget.updateT4();
    }

    void setCoordinate(int index, Eigen::Vector3f coordinate)
    {
        static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(index,1)->widget())->setValue(coordinate(0));
        static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(index,2)->widget())->setValue(coordinate(1));
        static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(index,3)->widget())->setValue(coordinate(2));
    }

    void updateCoordinates()
    {
        updateT4();

        setCoordinate(0, _mprWidget.t1());
        setCoordinate(1, _mprWidget.t2());
        setCoordinate(2, _mprWidget.t3());
        setCoordinate(3, _mprWidget.t4());
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
        _mprLayout {},
        _mprTitleLabel {"MPR (mulit planar reconstruction) configuration:"},
        _volumeInfoLabel {},
        _coordinateLayout{},
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

        _coordinateLayout.addWidget(new QLabel("Point 1"), 0, 0);
        _coordinateLayout.addWidget(new QSpinBox, 0, 1);
        _coordinateLayout.addWidget(new QSpinBox, 0, 2);
        _coordinateLayout.addWidget(new QSpinBox, 0, 3);

        _coordinateLayout.addWidget(new QLabel("Point 2"), 1, 0);
        _coordinateLayout.addWidget(new QSpinBox, 1, 1);
        _coordinateLayout.addWidget(new QSpinBox, 1, 2);
        _coordinateLayout.addWidget(new QSpinBox, 1, 3);

        _coordinateLayout.addWidget(new QLabel("Point 3"), 2, 0);
        _coordinateLayout.addWidget(new QSpinBox, 2, 1);
        _coordinateLayout.addWidget(new QSpinBox, 2, 2);
        _coordinateLayout.addWidget(new QSpinBox, 2, 3);

        _coordinateLayout.addWidget(new QLabel("Point 4"), 3, 0);
        _coordinateLayout.addWidget(new QSpinBox, 3, 1);
        _coordinateLayout.addWidget(new QSpinBox, 3, 2);
        _coordinateLayout.addWidget(new QSpinBox, 3, 3);

        // disable precomputed 4th point
        for(int i = 0; i<4; ++i)
        {
            _coordinateLayout.itemAtPosition(3,i)->widget()->setEnabled(false);
            if(i>0) // only spinboxes!
                static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(3,i)->widget())->setRange(-1000000000, 1000000000);
        }
        updateCoordinates();

        _mprTitleLabel.setMaximumHeight(50);
        _mprLayout.addWidget(&_mprTitleLabel);
        _mprLayout.addWidget(&_volumeInfoLabel);
        _mprLayout.addItem(&_coordinateLayout);

        _menuLayout.addItem(&_mprLayout);

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

        // t1 signal-slot functions
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(0,1)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT1X);
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(0,2)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT1Y);
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(0,3)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT1Z);

        // t2 signal-slot functions
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(1,1)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT2X);
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(1,2)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT2Y);
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(1,3)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT2Z);

        // t3 signal-slot functions
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(2,1)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT3X);
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(2,2)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT3Y);
        connect(static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(2,3)->widget()),
                    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this, &VisualizationWidget::updateT3Z);


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
        updateVolumeChanged();
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
            updateVolumeChanged();
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

    void updateT1X(int value)
    {
        Eigen::Vector3f t1 = _mprWidget.t1();
        _mprWidget.setT1(Eigen::Vector3f(value, t1(1), t1(2)));

        updateCoordinates();
    }

    void updateT1Y(int value)
    {
        Eigen::Vector3f t1 = _mprWidget.t1();
        _mprWidget.setT1(Eigen::Vector3f(t1(0), value, t1(2)));

        updateCoordinates();
    }

    void updateT1Z(int value)
    {
        Eigen::Vector3f t1 = _mprWidget.t1();
        _mprWidget.setT1(Eigen::Vector3f(t1(0), t1(1), value));

        updateCoordinates();
    }

    void updateT2X(int value)
    {
        Eigen::Vector3f t2 = _mprWidget.t2();
        _mprWidget.setT2(Eigen::Vector3f(value, t2(1), t2(2)));

        updateCoordinates();
    }

    void updateT2Y(int value)
    {
        Eigen::Vector3f t2 = _mprWidget.t2();
        _mprWidget.setT2(Eigen::Vector3f(t2(0), value, t2(2)));

        updateCoordinates();
    }

    void updateT2Z(int value)
    {
        Eigen::Vector3f t2 = _mprWidget.t2();
        _mprWidget.setT2(Eigen::Vector3f(t2(0), t2(1), value));

        updateCoordinates();
    }

    void updateT3X(int value)
    {
        Eigen::Vector3f t3 = _mprWidget.t3();
        _mprWidget.setT3(Eigen::Vector3f(value, t3(1), t3(2)));

        updateCoordinates();
    }

    void updateT3Y(int value)
    {
        Eigen::Vector3f t3 = _mprWidget.t3();
        _mprWidget.setT3(Eigen::Vector3f(t3(0), value, t3(2)));

        updateCoordinates();
    }

    void updateT3Z(int value)
    {
        Eigen::Vector3f t3 = _mprWidget.t3();
        _mprWidget.setT3(Eigen::Vector3f(t3(0), t3(1), value));

        updateCoordinates();
    }
};

#endif // VISUALIZATIONWIDGET_H
