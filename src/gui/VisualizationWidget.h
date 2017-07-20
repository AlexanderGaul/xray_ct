#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include <QAbstractSlider>
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

    
    ///loads the Volume from a file
    QPushButton *_loadFileButton;    
    ///loads the Volume from the ReconstructionPose
    QPushButton *_loadRecButton;
    ///displays the current color for mpr
    QLabel *_mprColorLabel;
    ///opens a dialog to choose the color for visualization for mpr
    QPushButton *_mprSelectColorButton;
    ///displays title of MPR
    QLabel *_mprTitleLabel;
    ///shows the dimensions of the volume on screen
    QLabel *_volumeInfoLabel;
    std::array<QSlider*, 3> _normalSlider;
    std::array<QDoubleSpinBox*, 3> _normalSpinBox;
    
    
    QSlider *_distanceSlider;
    QDoubleSpinBox *_distanceSpinBox;
    ///displays the current color for dvr
    QLabel *_dvrColorLabel;
    ///opens a dialog to choose the color for visualization for dvr
    QPushButton *_dvrSelectColorButton;
    ///display description of DVR
    QLabel *_dvrTitleLabel;
    ///info text regarding angle for DVR
    QLabel *_dvrAngleLabel;
    ///choose a suitable angle for DVR
    QSlider *_dvrAngleSlider;
    ///display current angle (in degrees)
    QSpinBox *_dvrAngleSpinBox;
    ///info text regarding angle for DVR
    QLabel *_dvrStepWidthLabel;
    ///choose a suitable step width
    QSlider *_dvrStepWidthSlider;
    ///display current step width
    QDoubleSpinBox *_dvrStepWidthSpinBox;
    ///renders the 2D MPR visualization
    MPRWidget *_mprWidget;
    ///paints the direct volume rendering (DVR)
    DVRWidget *_dvrWidget;

    /**
     * Updates (colors) the color label
     * to show the user the currently used
     * transfer function color for MPR.
     * @brief updateColorLabel
     */
    void mprUpdateColorLabel();

    /**
     * Updates (colors) the color label
     * to show the user the currently used
     * transfer function color for DVR.
     * @brief updateColorLabel
     */
    void dvrUpdateColorLabel();

    /**
     *  Updates the info label which
     *  displays the current volume size.
     */
    void updateVolumeInfo();

    /**
     * Helper method, called after change of volume data.
     * Updates the user interface accordingly.
     * @brief updateVolumeChanged
     */
    void updateVolumeChanged();

    /**
     * Reset the DRW widget to its default position and
     * calibrate the camera for a new volume.
     * @brief updateDRWWidget
     */
    void updateDVRWidget();

public:    
    /**
     * Construct a new Visualization Widget and setup
     * the whole user interface.
     * @brief VisualizationWidget
     */
    VisualizationWidget();
    
    /**
     * Updates the current volume.
     * This method makes it possible to load reconstruction
     * results into the visualization widget.
     * @brief setRec
     * @param vol - shared pointer to reference to new volume.
     */
    void setRec(const std::shared_ptr<const Volume>& vol);

    void updateNormalLayout()
    {
        Eigen::Vector3f normal = _mprWidget->normal();
        _normalSlider.at(0)->setValue(normal[0]);
        _normalSlider.at(1)->setValue(normal[1]);
        _normalSlider.at(2)->setValue(normal[2]);

        _normalSpinBox.at(0)->setValue(normal[0]);
        _normalSpinBox.at(1)->setValue(normal[1]);
        _normalSpinBox.at(2)->setValue(normal[2]);
    }
    
signals:
    /**
     * emit this event if you want the volume from the requisition. It is set through a call of setRec.
     */
    void requestRecVolume();
    
public slots:
    /**
     * Change color of transfer function of MPR.
     *
     * @brief mprChangeColor
     */
    void mprChangeColor();

    /**
     * Change color of transfer function of DVR.
     *
     * @brief dvrChangeColor
     */
    void dvrChangeColor();

    /**
     * Loads a 3D medical image file into a Volume.
     *
     * After that, the visualization widgets are reset to the default viewpoints.
     * @brief loadFromFile
     */
    void loadFromFile();

    /**
     * Updates the angle for DVR.
     * The angle is given in degrees.
     * @brief updateDVRAngle
     * @param angle
     */
    void updateDVRAngle(int angle);

    /**
     * Updates the step width for DVR.
     *
     * @brief updateDVRStep size
     * @param stepWidth
     */
    void updateDVRStepSize(float stepSize);

    void updateDistance(float distance)
    {
        _distanceSlider->setValue(distance);
        _distanceSpinBox->setValue(distance);
        _mprWidget->setDistance(distance);
    }

    /**
     * TODO: Make private
     * @brief updateNormal
     */
    void updateNormal(float x, float y, float z)
    {
        _mprWidget->setNormal(Eigen::Vector3f(x,y,z));
        updateNormalLayout();
    }

    void updateNormalX(float x)
    {
        updateNormal(x, _mprWidget->normal()[1], _mprWidget->normal()[2]);
    }

    void updateNormalY(float y)
    {
        updateNormal(_mprWidget->normal()[0], y, _mprWidget->normal()[2]);
    }

    void updateNormalZ(float z)
    {
        updateNormal(_mprWidget->normal()[0], _mprWidget->normal()[1], z);
    }
};

#endif // VISUALIZATIONWIDGET_H
