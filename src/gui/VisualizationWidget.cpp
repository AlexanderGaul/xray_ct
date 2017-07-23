#include "VisualizationWidget.h"

#include <iostream>
#include <QGroupBox>
#include <QMessageBox>


void VisualizationWidget::updateVolumeInfo()
{
    Eigen::Vector3i size = _visModel.volume().getNumVoxels();
    int x = size[0], y = size[1], z = size[2];
    _volumeInfoLabel->setText("Volume size: ("+QString::number(x)+","+QString::number(y)+","+QString::number(z)+")");
}

void VisualizationWidget::updateVolumeChanged()
{
    updateDVRWidget();
    updateVolumeInfo();
}

void VisualizationWidget::updateDVRWidget()
{
    //_dvrWidget->setAngle(0.0);
    //_dvrWidget->calibrateCamera();
    _dvrWidget->setColorRange(0, _visModel.volume().maxEntry());
}

VisualizationWidget::VisualizationWidget() :
    _visModel {},
    _loadFileButton {new QPushButton{"Load from file"}},
    _loadRecButton {new QPushButton{"Load reconstruction"}},
    _volumeInfoLabel {new QLabel{}},
    _mprWidget {new MPRWidget{_visModel}},
    _dvrWidget {new DVRWidget{_visModel}},
    _axisWidget {new AxisWidget {_visModel.getMPRModel()}},
    _dvrControlWidget {new DVRControlWidget(_visModel.getDVRModel())},
    _mprControlWidget {new MPRControlWidget(_visModel.getMPRModel())}
{
    ///all layout items for visulisation are composed in the main layout
    QHBoxLayout *mainLayout = new QHBoxLayout {};
    ///manages all menu items
    QGridLayout *menuLayout = new QGridLayout {};
    
    menuLayout->addWidget(_loadFileButton, 0, 0);
    menuLayout->addWidget(_loadRecButton, 0, 1);

    menuLayout->addWidget(_mprControlWidget, 1, 0, 1, 2);
    menuLayout->addWidget(_dvrControlWidget, 2, 0, 1, 2);
    
    mainLayout->addItem(menuLayout);
    QVBoxLayout *mprLayout = new QVBoxLayout {};
    mprLayout->addWidget(_mprWidget);
    mprLayout->addWidget(_axisWidget);
    mprLayout->setStretch(0, 5);
    mprLayout->setStretch(1, 1);
    
    mainLayout->addItem(mprLayout);
    //mainLayout->addWidget(_axisWidget);
    mainLayout->addWidget(_dvrWidget);
    
    mainLayout->setStretch(0, 3);
    mainLayout->setStretch(1, 5);
    mainLayout->setStretch(2, 5);

    setLayout(mainLayout);

    connect(_loadFileButton, &QPushButton::pressed, this, &VisualizationWidget::loadFromFile);
    connect(_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);

    connect(_mprControlWidget, &MPRControlWidget::requestTransferFunctionReset, this, &VisualizationWidget::resetMPRTransferFunction);
    connect(_dvrControlWidget, &DVRControlWidget::requestTransferFunctionReset, this, &VisualizationWidget::resetDVRTransferFunction);
}

void VisualizationWidget::setRec(const std::shared_ptr<const Volume>& vol){
    if(!vol){
        QMessageBox::warning(nullptr, "No Reconstruction Present or Invalid Reconstruction!", "You have to generate a valid reconstruction before it can be visualized!");
        return;
    }
    _visModel.setVolume(*vol);
    updateVolumeChanged();
}

void VisualizationWidget::resetMPRTransferFunction()
{
    _mprControlWidget->setTransferFunctionRange(0, _visModel.volume().maxEntry());
}

void VisualizationWidget::resetDVRTransferFunction()
{
    _dvrControlWidget->setTransferFunctionRange(0, _visModel.volume().maxEntry());
}

void VisualizationWidget::loadFromFile(){
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Medical image data (*.edf)");
    if(filename.isEmpty()){
        return;
    }
    
    try{
        _visModel.setVolume(EDFHandler::read(filename.toStdString()));
        updateVolumeChanged();
    } catch (std::invalid_argument){
        QMessageBox::warning(this, "Unable to Open File", "The file couldn't be read!");
    } catch (std::runtime_error){
        QMessageBox::warning(this, "Unable to Open File", "The file couldn't be read!");
    }
}
