#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QSlider>
#include <QTextEdit>
#include <QWidget>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "SliceWidget.h"
#include "Acquisition.h"

class ReconstructionViewer : public QWidget
{
    Q_OBJECT
private:
    QLabel *_statusView;
    QSlider *_axisSlider;
    SliceWidget *_sWidget;

    QPushButton *_loadButton;

    QLabel *_iterationLabel;
    QSlider *_iterationSlider;

    QCheckBox *_regCheckBox;
    QDoubleSpinBox *_regLambda;

    QPushButton *_updateButton;
    
    //input of the percentage of normally distributed noise
    QDoubleSpinBox *_noiseSpinBox;
    //used to mark wheter the error should actually be aplied
    QCheckBox *_noiseCheckBox;

public:

    ReconstructionViewer() 
    : _statusView {new QLabel {}}, 
    _axisSlider {new QSlider {}}, 
    _sWidget {new SliceWidget {}}, 
    _loadButton {new QPushButton{"Load acquisition"}}, 
    _iterationLabel{new QLabel {}}, _iterationSlider{new QSlider {}}, 
    _regCheckBox{new QCheckBox{"Regularized Reconstruction"}}, _regLambda{new QDoubleSpinBox {}}, 
    _updateButton{new QPushButton{"Update reconstruction"}}, 
    _noiseSpinBox{new QDoubleSpinBox{}}, _noiseCheckBox {new QCheckBox{"Activate Noise"}} {
        QGridLayout *layout = new QGridLayout {};
        QGridLayout *sliderLayout = new QGridLayout {};
        QHBoxLayout *controlLayout = new QHBoxLayout {};
        QHBoxLayout *iterationLayout = new QHBoxLayout {};
        QHBoxLayout *regLayout = new QHBoxLayout {};
        QHBoxLayout *noiseLayout = new QHBoxLayout {};

        updateStatus();
        controlLayout->addWidget(_statusView);

        _axisSlider->setRange(1, 3);
        _axisSlider->setOrientation(Qt::Horizontal);
        _axisSlider->setValue(3); // Z is default direction
        _axisSlider->setSingleStep(1);
        QLabel *label1 = new QLabel("X", this);
        QLabel *label2 = new QLabel("Y", this);
        QLabel *label3 = new QLabel("Z", this);
        sliderLayout->addWidget(_axisSlider, 0, 0, 1, 2);

        sliderLayout->addWidget(label1, 1, 0, 1, 1);
        sliderLayout->addWidget(label2, 1, 1, 1, 1);
        sliderLayout->addWidget(label3, 1, 2, 1, 1);
        controlLayout->addItem(sliderLayout);

        layout->setRowStretch(0, 3);
        layout->setRowStretch(1, 1);

        layout->addWidget(_sWidget);
        layout->addItem(controlLayout, 1, 0);
        layout->addWidget(_loadButton, 2, 0);

        iterationLayout->addWidget(_iterationLabel);
        _iterationSlider->setRange(1, 100);
        _iterationSlider->setOrientation(Qt::Horizontal);
        //TODO: declare 5 as constant
        _iterationSlider->setValue(5);
        iterationLayout->addWidget(_iterationSlider);

        layout->addItem(iterationLayout, 3, 0);

        regLayout->addWidget(_regCheckBox);
        updateRegText();
        _regLambda->setButtonSymbols(QAbstractSpinBox::NoButtons);
        regLayout->addWidget(_regLambda);
        _regLambda->setRange(0, 100000);
        layout->addItem(regLayout, 4, 0);

        _updateButton->setEnabled(false);
        layout->addWidget(_updateButton, 5, 0);
        noiseLayout->addWidget(_noiseCheckBox);
        noiseLayout->addWidget(_noiseSpinBox);
        layout->addItem(noiseLayout, 6, 0);

        setLayout(layout);

        connect(_sWidget, &SliceWidget::sliceChanged, this, &ReconstructionViewer::updateStatus);
        connect(_axisSlider, &QSlider::valueChanged, _sWidget, &SliceWidget::updateStatus);
        connect(_loadButton, &QPushButton::pressed, this, &ReconstructionViewer::requestAcquisition);

        connect(_iterationSlider, &QSlider::valueChanged, this, &ReconstructionViewer::updateStatus);
        connect(_regCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::updateRegText);

        connect(_iterationSlider, &QSlider::valueChanged, this, &ReconstructionViewer::enableUpdate);
        connect(_regCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::enableUpdate);
        connect(_regLambda, &QDoubleSpinBox::editingFinished, this, &ReconstructionViewer::enableUpdate);
        connect(_noiseSpinBox, &QDoubleSpinBox::editingFinished, this,
                &ReconstructionViewer::enableUpdate);
        connect(_noiseCheckBox, &QCheckBox::stateChanged, this,
                &ReconstructionViewer::enableUpdate);
        connect(_noiseCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::updateSpinBox);

        connect(_updateButton, &QPushButton::pressed, this, &ReconstructionViewer::updateReconstruction);
        _noiseSpinBox->setMaximum(1000.);
        _noiseSpinBox->setMinimum(0.);
        _noiseSpinBox->setDecimals(2);
        _noiseSpinBox->setSuffix("%");
        _noiseSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        _noiseSpinBox->setKeyboardTracking(false);
        
        updateRegText();
        updateSpinBox();
        
    }

    void setAcq(Acquisition&& acq){
            _sWidget->setAcq(_regCheckBox->isChecked(),
                            static_cast<float>(_regLambda->value()),
                            _noiseCheckBox->isChecked(),
                            _noiseSpinBox->value()/100,
                            _iterationSlider->value(),
                            std::move(acq));
    }
    
    //on request returns the volume
    std::shared_ptr<const Volume> getRec(){
        return _sWidget->getRec();
    }
    
    
signals:
    //calls this function if you need the acquistion information. it is set through setAcq
    void requestAcquisition();
    
public slots:    

    /**
     * This method ensures that it is only possible to
     * enter a lambda into the corresponding text field,
     * if the regularization mode was enabled.
     * @brief updateRegText
     */
    void updateRegText()
    {
        _regLambda->setEnabled(_regCheckBox->isChecked());
    }
    
    void updateSpinBox(){
        _noiseSpinBox->setEnabled(_noiseCheckBox->isChecked());
    }

    void updateStatus() {
        _statusView->setText(QString("Current slice: "+QString::number(_sWidget->currSlice()+1)+
                                    " of "+QString::number(_sWidget->slices())));
        _iterationLabel->setText(QString("Iterations: "+QString::number(_iterationSlider->value())));
    }

    void updateReconstruction()
    {
        _sWidget->recParamChanged(_regCheckBox->isChecked(), _regLambda->value(),
                                 _noiseCheckBox->isChecked(), _noiseSpinBox->value()/100,
                                 _iterationSlider->value());
        //_updateButton.setEnabled(false);
    }

    /**
     * This slot makes it possible to update the reconstruction.
     * This method is called after the reconstruction parameters
     * were changed.
     * @brief enableUpdate
     */
    void enableUpdate()
    {
        if(!_sWidget->containsRec()){
            return;
        }
        _updateButton->setEnabled(true);
    }
};
