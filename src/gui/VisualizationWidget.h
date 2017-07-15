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

        _menuLayout.addWidget(&_loadFileButton);
        _menuLayout.addWidget(&_loadRecButton);
        _mainLayout.addItem(&_menuLayout, 0, 0);

        _mainLayout.addWidget(&_mprWidget, 0, 1);
        _mainLayout.addWidget(&_dvrWidget, 0, 2);

        _mprWidget.setMinimumHeight(150);
        setLayout(&_mainLayout);

        connect(&_granularitySlider, &QSlider::valueChanged, &_mprWidget, &MPRWidget::setGranularity);
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
};

#endif // VISUALIZATIONWIDGET_H
