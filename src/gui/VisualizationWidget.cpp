#include "VisualizationWidget.h"

#include <iostream>
#include <QGroupBox>
#include <QMessageBox>

void VisualizationWidget::mprUpdateColorLabel()
{
    _mprColorLabel->setStyleSheet( "background-color: " + _mprWidget->color().name() );
}

void VisualizationWidget::dvrUpdateColorLabel()
{
    _dvrColorLabel->setStyleSheet( "background-color: " + _dvrWidget->color().name() );
}

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
    _mprColorLabel {new QLabel {}},
    _mprSelectColorButton {new QPushButton{"Select color for MPR"}},
    _volumeInfoLabel {new QLabel{}},
    _dvrColorLabel {new QLabel{}},
    _dvrSelectColorButton {new QPushButton{"Select color for DVR"}},
    _mprWidget {new MPRWidget{_visModel}},
    _dvrWidget {new DVRWidget{_visModel}},
    
    _dvrControlWidget {new DVRControlWidget(_visModel.getDVRModel())},
    _mprControlWidget {new MPRControlWidget(_visModel.getMPRModel())}
{
    ///all layout items for visulisation are composed in the main layout
    QHBoxLayout *mainLayout = new QHBoxLayout {};
    ///manages all menu items
    QGridLayout *menuLayout = new QGridLayout {};
    ///manages the color-choosing related widgets for mpr
    QHBoxLayout *mprColorLayout = new QHBoxLayout {};
    ///manages the color-choosing related widgets for dvr
    QHBoxLayout *dvrColorLayout = new QHBoxLayout {};
    ///manages label config of DVR
    
    mprUpdateColorLabel();
    mprColorLayout->addWidget(_mprColorLabel);
    mprColorLayout->addWidget(_mprSelectColorButton);

    dvrUpdateColorLabel();
    dvrColorLayout->addWidget(_dvrColorLabel);
    dvrColorLayout->addWidget(_dvrSelectColorButton);
    
    
    menuLayout->addWidget(_loadFileButton, 0, 0);
    menuLayout->addWidget(_loadRecButton, 1, 0);
    
    
    //_dvrControlWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    menuLayout->addWidget(_mprControlWidget, 2, 0);
    menuLayout->addWidget(_dvrControlWidget, 3, 0);
    
    //_dvrWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //_mprWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addItem(menuLayout);
    mainLayout->addWidget(_mprWidget);
    mainLayout->addWidget(_dvrWidget);
    
    mainLayout->setStretch(0, 3);
    mainLayout->setStretch(1, 5);
    mainLayout->setStretch(2, 5);

    setLayout(mainLayout);


    connect(_loadFileButton, &QPushButton::pressed, this, &VisualizationWidget::loadFromFile);
    connect(_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);
    connect(_mprSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::mprChangeColor);
    connect(_dvrSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::dvrChangeColor);
}

void VisualizationWidget::setRec(const std::shared_ptr<const Volume>& vol){
    if(!vol){
        QMessageBox::warning(this, "No Reconstruction Present!", "You have to generate a reconstruction before it can be visualized!");
        return;
    }
    _visModel.setVolume(*vol);
    updateVolumeChanged();
}

void VisualizationWidget::mprChangeColor()
{
    QColor newColor = QColorDialog::getColor(_mprWidget->color(),parentWidget());
    if ( newColor != _mprWidget->color() )
    {
        _mprWidget->setColor( newColor );
        mprUpdateColorLabel();
    }
}

void VisualizationWidget::dvrChangeColor()
{
    QColor newColor = QColorDialog::getColor(_dvrWidget->color(), parentWidget());
    if ( newColor != _dvrWidget->color() )
    {
        _dvrWidget->setColor( newColor );
        dvrUpdateColorLabel();
    }
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