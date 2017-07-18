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
    ///manages the color-choosing related widgets for mpr
    QHBoxLayout _mprColorLayout;
    ///displays the current color for mpr
    QLabel _mprColorLabel;
    ///opens a dialog to choose the color for visualization for mpr
    QPushButton _mprSelectColorButton;
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
    ///manages the color-choosing related widgets for dvr
    QHBoxLayout _dvrColorLayout;
    ///displays the current color for dvr
    QLabel _dvrColorLabel;
    ///opens a dialog to choose the color for visualization for dvr
    QPushButton _dvrSelectColorButton;
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
    QDoubleSpinBox _dvrStepWidthSpinBox;
    ///renders the 2D MPR visualization
    MPRWidget _mprWidget;
    ///paints the direct volume rendering (DVR)
    DVRWidget _dvrWidget;

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
     * Sets the range limit of a certain spinbox inside
     * the coordinate input layout.
     * The spinbox gets the range [0,limit].
     * @brief setLimit
     * @param x - determines index of Point (from 0 to 3)
     * @param y - determines dimension where the limit is set
     * (x = 1, y = 2, or z = 3)
     * @param limit
     */
    void setLimit(int x, int y, int limit);

    /**
     * Helper method, updates the limits
     * for entering coordinates according to
     * the current volume size.
     * @brief updateCoordinateLimits
     */
    void updateCoordinateLimits();

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

    /**
     * Calculate the fourth edge point of the plane used
     * in multiple planar reconstruction (MPR).
     */
    void updateT4();

    /**
     * Updates on-screen value to given coordinate.
     * Index parameter is used to access the spinboxes inside
     * the coordinate grid layout.
     * @brief setCoordinate
     * @param index - index of point to be updated (can vary between 0 and 3, inclusive).
     * @param coordinate
     */
    void setCoordinate(int index, Eigen::Vector3f coordinate);

    /**
     * Updates the coordinates of the control point T4
     * and additionally updates on-screen values of T1,T2,T3
     * @brief updateCoordinates
     */
    void updateCoordinates();
    
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

    /**
     * Updates the X value of the first coordinate
     * determining the cutting plane (t1).
     * @brief updateT1X
     * @param value - new value of the X coordinate
     */
    void updateT1X(int value);

    /**
     * Updates the Y value of the first coordinate
     * determining the cutting plane (t1).
     * @brief updateT1Y
     * @param value - new value of the Y coordinate
     */
    void updateT1Y(int value);

    /**
     * Updates the Z value of the first coordinate
     * determining the cutting plane (t1).
     * @brief updateT1Z
     * @param value - new value of the Z coordinate
     */
    void updateT1Z(int value);

    /**
     * Updates the X value of the second coordinate
     * determining the cutting plane (t2).
     * @brief updateT2X
     * @param value - new value of the X coordinate
     */
    void updateT2X(int value);

    /**
     * Updates the Y value of the second coordinate
     * determining the cutting plane (t2).
     * @brief updateT2Y
     * @param value - new value of the Y coordinate
     */
    void updateT2Y(int value);

    /**
     * Updates the Z value of the second coordinate
     * determining the cutting plane (t2).
     * @brief updateT2Z
     * @param value - new value of the Z coordinate
     */
    void updateT2Z(int value);

    /**
     * Updates the X value of the third coordinate
     * determining the cutting plane (t3).
     * @brief updateT3X
     * @param value - new value of the X coordinate
     */
    void updateT3X(int value);

    /**
     * Updates the Y value of the third coordinate
     * determining the cutting plane (t3).
     * @brief updateT3Y
     * @param value - new value of the Y coordinate
     */
    void updateT3Y(int value);
    /**
     * Updates the Z value of the third coordinate
     * determining the cutting plane (t3).
     * @brief updateT3Z
     * @param value - new value of the Z coordinate
     */
    void updateT3Z(int value);
};

#endif // VISUALIZATIONWIDGET_H
