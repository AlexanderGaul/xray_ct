#include "VisualizationWidget.h"

void VisualizationWidget::mprUpdateColorLabel()
{
    _mprColorLabel.setStyleSheet( "background-color: " + _mprWidget.color().name() );
}

void VisualizationWidget::dvrUpdateColorLabel()
{
    _dvrColorLabel.setStyleSheet( "background-color: " + _dvrWidget.color().name() );
}

void VisualizationWidget::updateVolumeInfo()
{
    Eigen::Vector3i size = _visModel.volume().getNumVoxels();
    int x = size[0], y = size[1], z = size[2];
    _volumeInfoLabel.setText("Volume size: ("+QString::number(x)+","+QString::number(y)+","+QString::number(z)+")");
}

void VisualizationWidget::updateVolumeChanged()
{
    updateDVRWidget();
    updateVolumeInfo();
}

void VisualizationWidget::updateDVRWidget()
{
    _dvrWidget.setAngle(0.0);
    _dvrWidget.calibrateCamera();
    _dvrWidget.setColorRange(0, _visModel.volume().maxEntry());
}

VisualizationWidget::VisualizationWidget() :
    _visModel {},
    _mainLayout {},
    _menuLayout {},
    _loadFileButton {"Load from file"},
    _loadRecButton {"Load reconstruction"},
    _mprColorLayout {},
    _mprColorLabel {},
    _mprSelectColorButton {"Select color for MPR"},
    _mprLayout {},
    _mprTitleLabel {"MPR (mulit planar reconstruction) configuration:"},
    _volumeInfoLabel {},
    _normalVectorLayout {},
    _mprDistanceLayout {},
    _distanceSlider {},
    _distanceSpinBox {},
    _dvrLayout {},
    _dvrColorLayout {},
    _dvrColorLabel {},
    _dvrSelectColorButton {"Select color for DVR"},
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
    _mprTitleLabel.setMaximumHeight(50);
    mprUpdateColorLabel();
    _mprColorLayout.addWidget(&_mprColorLabel);
    _mprColorLayout.addWidget(&_mprSelectColorButton);
    _mprLayout.addItem(&_mprColorLayout);
    _mprLayout.addWidget(&_mprTitleLabel);
    _mprLayout.addWidget(&_volumeInfoLabel);

    _normalVectorLayout.addWidget(new QLabel("Normal Vector: "));
    for(int i = 0; i<3; ++i)
    {
        QHBoxLayout* layout = new QHBoxLayout;
        QSlider* slider = new QSlider;
        slider->setOrientation(Qt::Horizontal);
        slider->setMaximumWidth(200);
        QDoubleSpinBox* dSpinBox = new QDoubleSpinBox;
        dSpinBox->setMaximumWidth(200);

        layout->addWidget(slider);
        layout->addWidget(dSpinBox);

        _normalVectorLayout.addItem(layout);
    }

    _mprLayout.addItem(&_normalVectorLayout);

    QLabel* distanceLabel = new QLabel("Distance: ");
    distanceLabel->setMaximumWidth(200);
    _mprDistanceLayout.addWidget(distanceLabel);
    _distanceSlider.setOrientation(Qt::Horizontal);
    _distanceSlider.setMaximumWidth(200);
    _mprDistanceLayout.addWidget(&_distanceSlider);
    _distanceSpinBox.setMaximumWidth(200);
    _mprDistanceLayout.addWidget(&_distanceSpinBox);

    _mprLayout.addItem(&_mprDistanceLayout);

    _menuLayout.addItem(&_mprLayout);

    dvrUpdateColorLabel();
    _dvrColorLayout.addWidget(&_dvrColorLabel);
    _dvrColorLayout.addWidget(&_dvrSelectColorButton);

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
    _mprLayout.addItem(&_dvrColorLayout);
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

    connect(static_cast<QAbstractSlider*>(_normalVectorLayout.itemAt(1)->layout()->itemAt(0)->widget()), &QSlider::valueChanged, this, &VisualizationWidget::updateNormalX);
    connect(static_cast<QAbstractSlider*>(_normalVectorLayout.itemAt(2)->layout()->itemAt(0)->widget()), &QSlider::valueChanged, this, &VisualizationWidget::updateNormalY);
    connect(static_cast<QAbstractSlider*>(_normalVectorLayout.itemAt(3)->layout()->itemAt(0)->widget()), &QSlider::valueChanged, this, &VisualizationWidget::updateNormalZ);

    connect(static_cast<QDoubleSpinBox*>(_normalVectorLayout.itemAt(1)->layout()->itemAt(1)->widget()),
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateNormalX);
    connect(static_cast<QDoubleSpinBox*>(_normalVectorLayout.itemAt(2)->layout()->itemAt(1)->widget()),
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateNormalY);
    connect(static_cast<QDoubleSpinBox*>(_normalVectorLayout.itemAt(3)->layout()->itemAt(1)->widget()),
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateNormalZ);

    connect(&_distanceSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDistance);
    connect(&_distanceSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateDistance);

    connect(&_dvrAngleSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDVRAngle);
    // cast necessary because spinbox has also valueChanged(QString) signal
    connect(&_dvrAngleSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &VisualizationWidget::updateDVRAngle);
    connect(&_dvrStepWidthSlider, &QSlider::valueChanged, this, &VisualizationWidget::updateDVRStepSize);
    connect(&_dvrStepWidthSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VisualizationWidget::updateDVRStepSize);
    connect(&_loadFileButton, &QPushButton::pressed, this, &VisualizationWidget::loadFromFile);
    connect(&_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);
    connect(&_mprSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::mprChangeColor);
    connect(&_dvrSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::dvrChangeColor);
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
    QColor newColor = QColorDialog::getColor(_mprWidget.color(),parentWidget());
    if ( newColor != _mprWidget.color() )
    {
        _mprWidget.setColor( newColor );
        mprUpdateColorLabel();
    }
}

void VisualizationWidget::dvrChangeColor()
{
    QColor newColor = QColorDialog::getColor(_dvrWidget.color(),parentWidget());
    if ( newColor != _dvrWidget.color() )
    {
        _dvrWidget.setColor( newColor );
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
    _dvrAngleSpinBox.setValue(angle);
    _dvrAngleSlider.setValue(angle);

    float radianAngle = (angle*M_PI)/180.0;
    _dvrWidget.setAngle(radianAngle);
}

void VisualizationWidget::updateDVRStepSize(float stepSize)
{
    _dvrStepWidthSpinBox.setValue(stepSize);
    _dvrStepWidthSlider.setValue(stepSize);

    _dvrWidget.setStepSize(stepSize);
}
