#include "VisualizationWidget.h"

void VisualizationWidget::updateColorLabel()
{
    _colorLabel.setStyleSheet( "background-color: " + _visModel.color().name() );
}

void VisualizationWidget::updateVolumeInfo()
{
    Eigen::Vector3i size = _visModel.volume().getNumVoxels();
    int x = size[0], y = size[1], z = size[2];
    _volumeInfoLabel.setText("Volume size: ("+QString::number(x)+","+QString::number(y)+","+QString::number(z)+")");
}

void VisualizationWidget::setLimit(int x, int y, int limit)
{
    static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(x, y)->widget())->setRange(0,limit-1);
}

void VisualizationWidget::updateCoordinateLimits()
{
    Eigen::Vector3i size = _visModel.volume().getNumVoxels();
    for(int i = 0; i<3; ++i)
    {
        for(int j = 0; j<3; ++j)
        {
            setLimit(j,(i+1),size[i]);
        }
    }
}

void VisualizationWidget::updateVolumeChanged()
{
    updateDVRWidget();
    updateVolumeInfo();
    updateCoordinateLimits();
}

void VisualizationWidget::updateDVRWidget()
{
    _dvrWidget.setAngle(0.0);
    _dvrWidget.calibrateCamera();
}

/**
 * Calculate the fourth edge point of the plane used
 * in multiple planar reconstruction (MPR).
 */
void VisualizationWidget::updateT4()
{
    _mprWidget.updateT4();
}

void VisualizationWidget::setCoordinate(int index, Eigen::Vector3f coordinate)
{
    static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(index,1)->widget())->setValue(coordinate(0));
    static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(index,2)->widget())->setValue(coordinate(1));
    static_cast<QSpinBox*>(_coordinateLayout.itemAtPosition(index,3)->widget())->setValue(coordinate(2));
}

void VisualizationWidget::updateCoordinates()
{
    updateT4();

    setCoordinate(0, _mprWidget.t1());
    setCoordinate(1, _mprWidget.t2());
    setCoordinate(2, _mprWidget.t3());
    setCoordinate(3, _mprWidget.t4());
}

VisualizationWidget::VisualizationWidget() :
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

void VisualizationWidget::setRec(const std::shared_ptr<const Volume>& vol){
    _visModel.setVolume(*vol);
    updateVolumeChanged();
}

void VisualizationWidget::changeColor()
{
    QColor newColor = QColorDialog::getColor(_visModel.color(),parentWidget());
    if ( newColor != _visModel.color() )
    {
        _visModel.setColor( newColor );
        updateColorLabel();
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

void VisualizationWidget::updateDVRStepWidth(int stepWidth)
{
    _dvrStepWidthSpinBox.setValue(stepWidth);
    _dvrStepWidthSlider.setValue(stepWidth);

    //TODO: update value in dvr widget
}

void VisualizationWidget::updateT1X(int value)
{
    Eigen::Vector3f t1 = _mprWidget.t1();
    _mprWidget.setT1(Eigen::Vector3f(value, t1(1), t1(2)));

    updateCoordinates();
}

void VisualizationWidget::updateT1Y(int value)
{
    Eigen::Vector3f t1 = _mprWidget.t1();
    _mprWidget.setT1(Eigen::Vector3f(t1(0), value, t1(2)));

    updateCoordinates();
}

void VisualizationWidget::updateT1Z(int value)
{
    Eigen::Vector3f t1 = _mprWidget.t1();
    _mprWidget.setT1(Eigen::Vector3f(t1(0), t1(1), value));

    updateCoordinates();
}

void VisualizationWidget::updateT2X(int value)
{
    Eigen::Vector3f t2 = _mprWidget.t2();
    _mprWidget.setT2(Eigen::Vector3f(value, t2(1), t2(2)));

    updateCoordinates();
}

void VisualizationWidget::updateT2Y(int value)
{
    Eigen::Vector3f t2 = _mprWidget.t2();
    _mprWidget.setT2(Eigen::Vector3f(t2(0), value, t2(2)));

    updateCoordinates();
}

void VisualizationWidget::updateT2Z(int value)
{
    Eigen::Vector3f t2 = _mprWidget.t2();
    _mprWidget.setT2(Eigen::Vector3f(t2(0), t2(1), value));

    updateCoordinates();
}

void VisualizationWidget::updateT3X(int value)
{
    Eigen::Vector3f t3 = _mprWidget.t3();
    _mprWidget.setT3(Eigen::Vector3f(value, t3(1), t3(2)));

    updateCoordinates();
}

void VisualizationWidget::updateT3Y(int value)
{
    Eigen::Vector3f t3 = _mprWidget.t3();
    _mprWidget.setT3(Eigen::Vector3f(t3(0), value, t3(2)));

    updateCoordinates();
}

void VisualizationWidget::updateT3Z(int value)
{
    Eigen::Vector3f t3 = _mprWidget.t3();
    _mprWidget.setT3(Eigen::Vector3f(t3(0), t3(1), value));

    updateCoordinates();
}
