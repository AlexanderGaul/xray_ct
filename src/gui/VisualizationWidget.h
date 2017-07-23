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

#include "DVRControlWidget.h"
#include "MPRControlWidget.h"
#include "AxisWidget.h"

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
    ///shows the dimensions of the volume on screen
    QLabel *_volumeInfoLabel;
    
    ///renders the 2D MPR visualization
    MPRWidget *_mprWidget;
    ///paints the direct volume rendering (DVR)
    DVRWidget *_dvrWidget;
    
    ///paints the axes for the mpr to visualize the current rotation
    AxisWidget *_axisWidget;
    
    DVRControlWidget* _dvrControlWidget;
    MPRControlWidget* _mprControlWidget;
    


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
    void resetMPRTransferFunction();

    void resetDVRTransferFunction();

    /**
     * Loads a 3D medical image file into a Volume.
     *
     * @brief loadFromFile
     */
    void loadFromFile();

};

#endif // VISUALIZATIONWIDGET_H
