#include "VisualizationWidget.h"

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
    _mprTitleLabel {new QLabel{"MPR (mulit planar reconstruction) configuration:"}},
    _volumeInfoLabel {new QLabel{}},
    _normalSlider {{new QSlider{}, new QSlider{}, new QSlider{}}},
    _normalSpinBox {{new QDoubleSpinBox{}, new QDoubleSpinBox{}, new QDoubleSpinBox{}}},
    _distanceSlider {new QSlider {}},
    _distanceSpinBox {new QDoubleSpinBox{}},
    _dvrColorLabel {new QLabel{}},
    _dvrSelectColorButton {new QPushButton{"Select color for DVR"}},
    _dvrTitleLabel {new QLabel{"DVR (direct volume rendering) configuration:"}},
    _dvrAngleLabel {new QLabel{"Angle: "}},
    _dvrAngleSlider {new QSlider {}},
    _dvrAngleSpinBox {new QSpinBox {}},
    _dvrStepWidthLabel {new QLabel{"Step width: "}},
    _dvrStepWidthSlider {new QSlider {}},
    _dvrStepWidthSpinBox {new QDoubleSpinBox {}},
    _mprWidget {new MPRWidget{_visModel}},
    _dvrWidget {new DVRWidget{_visModel}}
{
    ///all layout items for visulisation are composed in the main layout
    QGridLayout *mainLayout = new QGridLayout {};
    ///manages all menu items
    QVBoxLayout *menuLayout = new QVBoxLayout {};
    ///manages the color-choosing related widgets for mpr
    QHBoxLayout *mprColorLayout = new QHBoxLayout {};
    ///manages the MPR configuration
    QVBoxLayout *mprLayout = new QVBoxLayout {};
    ///layout for entering normal vector
    QVBoxLayout *normalVectorLayout = new QVBoxLayout {};
    ///layout for entering distance of plane in mpr
    QHBoxLayout *mprDistanceLayout = new QHBoxLayout {};
    ///manages configuration of DVR
    QVBoxLayout *dvrLayout = new QVBoxLayout {};
    ///manages the color-choosing related widgets for dvr
    QHBoxLayout *dvrColorLayout = new QHBoxLayout {};
    ///manages label config of DVR
    QHBoxLayout *dvrAngleLayout = new QHBoxLayout {};
    ///manages step width config of DVR
    QHBoxLayout *dvrStepWidthLayout = new QHBoxLayout {};
    
    _mprTitleLabel->setMaximumHeight(50);
    mprUpdateColorLabel();
    mprColorLayout->addWidget(_mprColorLabel);
    mprColorLayout->addWidget(_mprSelectColorButton);
    mprLayout->addItem(mprColorLayout);
    mprLayout->addWidget(_mprTitleLabel);
    mprLayout->addWidget(_volumeInfoLabel);

    normalVectorLayout->addWidget(new QLabel("Normal Vector: "));
    for(int i = 0; i<3; ++i)
    {
        QHBoxLayout* layout = new QHBoxLayout;
        _normalSlider.at(i)->setOrientation(Qt::Horizontal);
        _normalSlider.at(i)->setMaximumWidth(200);
        _normalSpinBox.at(i)->setMaximumWidth(200);

        layout->addWidget(_normalSlider.at(i));
        layout->addWidget(_normalSpinBox.at(i));

        normalVectorLayout->addItem(layout);
    }

    mprLayout->addItem(normalVectorLayout);

    QLabel* distanceLabel = new QLabel("Distance: ");
    distanceLabel->setMaximumWidth(200);
    mprDistanceLayout->addWidget(distanceLabel);
    _distanceSlider->setOrientation(Qt::Horizontal);
    _distanceSlider->setMaximumWidth(200);
    mprDistanceLayout->addWidget(_distanceSlider);
    _distanceSpinBox->setMaximumWidth(200);
    mprDistanceLayout->addWidget(_distanceSpinBox);

    mprLayout->addItem(mprDistanceLayout);

    menuLayout->addItem(mprLayout);

    dvrUpdateColorLabel();
    dvrColorLayout->addWidget(_dvrColorLabel);
    dvrColorLayout->addWidget(_dvrSelectColorButton);

    dvrAngleLayout->addWidget(_dvrAngleLabel);
    _dvrAngleSlider->setRange(1,360);
    _dvrAngleSlider->setOrientation(Qt::Horizontal);
    dvrAngleLayout->addWidget(_dvrAngleSlider);
    _dvrAngleSpinBox->setRange(1,360);
    dvrAngleLayout->addWidget(_dvrAngleSpinBox);
    _dvrAngleLabel->setMaximumWidth(200);
    _dvrAngleSlider->setMaximumWidth(200);
    _dvrAngleSpinBox->setMaximumWidth(200);

    dvrStepWidthLayout->addWidget(_dvrStepWidthLabel);
    _dvrStepWidthSlider->setRange(1,100);
    _dvrStepWidthSlider->setOrientation(Qt::Horizontal);
    dvrStepWidthLayout->addWidget(_dvrStepWidthSlider);
    dvrStepWidthLayout->addWidget(_dvrStepWidthSpinBox);
    _dvrStepWidthLabel->setMaximumWidth(200);
    _dvrStepWidthSlider->setMaximumWidth(200);
    _dvrStepWidthSpinBox->setMaximumWidth(200);

    dvrLayout->addWidget(_dvrTitleLabel);
    mprLayout->addItem(dvrColorLayout);
    dvrLayout->addItem(dvrAngleLayout);
    dvrLayout->addItem(dvrStepWidthLayout);

    menuLayout->addItem(dvrLayout);

    menuLayout->addWidget(_loadFileButton);
    menuLayout->addWidget(_loadRecButton);
    mainLayout->addItem(menuLayout, 0, 0);

    mainLayout->addWidget(_mprWidget, 0, 1);
    mainLayout->addWidget(_dvrWidget, 0, 2);

    _mprWidget->setMinimumHeight(150);
    setLayout(mainLayout);

    connect(_normalSlider.at(0), &QSlider::valueChanged, this, &VisualizationWidget::updateNormalX);
    connect(_normalSlider.at(1), &QSlider::valueChanged, this, &VisualizationWidget::updateNormalY);
    connect(_normalSlider.at(2), &QSlider::valueChanged, this, &VisualizationWidget::updateNormalZ);

    connect(_normalSpinBox.at(0),
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateNormalX);
    connect(_normalSpinBox.at(1),
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateNormalY);
    connect(_normalSpinBox.at(2),
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateNormalZ);

    connect(_distanceSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDistance);
    connect(_distanceSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateDistance);

    connect(_dvrAngleSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDVRAngle);
    // cast necessary because spinbox has also valueChanged(QString) signal
    connect(_dvrAngleSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &VisualizationWidget::updateDVRAngle);
    connect(_dvrStepWidthSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDVRStepSize);
    connect(_dvrStepWidthSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateDVRStepSize);
    connect(_loadFileButton, &QPushButton::pressed, this, &VisualizationWidget::loadFromFile);
    connect(_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);
    connect(_mprSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::mprChangeColor);
    connect(_dvrSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::dvrChangeColor);
}

void VisualizationWidget::setRec(const std::shared_ptr<const Volume>& vol){
    if(!vol){
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

void VisualizationWidget::updateDVRAngle(int angle)
{
    _dvrAngleSpinBox->setValue(angle);
    _dvrAngleSlider->setValue(angle);

    float radianAngle = (angle*M_PI)/180.0;
    //_dvrWidget->setAngle(radianAngle);
}

void VisualizationWidget::updateDVRStepSize(float stepSize)
{
    _dvrStepWidthSpinBox->setValue(stepSize);
    _dvrStepWidthSlider->setValue(stepSize);

    _dvrWidget->setStepSize(stepSize);
}
