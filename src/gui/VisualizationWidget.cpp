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
}

void VisualizationWidget::setRec(const std::shared_ptr<const Volume>& vol){
    if(!vol){
        QMessageBox::warning(this, "No Reconstruction Present!", "You have to generate a reconstruction before it can be visualized!");
        return;
    }
    _visModel.setVolume(*vol);
    updateVolumeChanged();
}


void VisualizationWidget::loadFromFile(){
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
