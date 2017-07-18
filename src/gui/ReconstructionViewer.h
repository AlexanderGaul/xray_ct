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
    QGridLayout _layout;
    QHBoxLayout _controlLayout;
    QLabel _statusView;
    QGridLayout _sliderLayout;
    QSlider _axisSlider;
    SliceWidget _sWidget;

    QPushButton _loadButton;

    QHBoxLayout _iterationLayout;
    QLabel _iterationLabel;
    QSlider _iterationSlider;

    QHBoxLayout _regLayout;
    QCheckBox _regCheckBox;
    QDoubleSpinBox _regLambda;

    QPushButton _updateButton;
    
    QHBoxLayout _noiseLayout;
    //input of the percentage of normally distributed noise
    QDoubleSpinBox _noiseSpinBox;
    //used to mark wheter the error should actually be aplied
    QCheckBox _noiseCheckBox;

public:

    ReconstructionViewer() : _layout {}, _controlLayout {}, _statusView {}, _sliderLayout {},
    _axisSlider {}, _sWidget {}, _loadButton {"Load acquisition"}, _iterationLayout{},
    _iterationLabel{}, _iterationSlider{}, _regLayout{}, _regCheckBox{"Regularized Reconstruction"}, _regLambda{},
    _updateButton{"Update reconstruction"}, 
    _noiseLayout{}, _noiseSpinBox{}, _noiseCheckBox {"Activate Noise"}{
        updateStatus();
        _controlLayout.addWidget(&_statusView);

        _axisSlider.setRange(1, 3);
        _axisSlider.setOrientation(Qt::Horizontal);
        _axisSlider.setValue(3); // Z is default direction
        _axisSlider.setSingleStep(1);
        QLabel *label1 = new QLabel("X", this);
        QLabel *label2 = new QLabel("Y", this);
        QLabel *label3 = new QLabel("Z", this);
        _sliderLayout.addWidget(&_axisSlider, 0, 0, 1, 2);

        _sliderLayout.addWidget(label1, 1, 0, 1, 1);
        _sliderLayout.addWidget(label2, 1, 1, 1, 1);
        _sliderLayout.addWidget(label3, 1, 2, 1, 1);
        _controlLayout.addItem(&_sliderLayout);

        _layout.setRowStretch(0, 3);
        _layout.setRowStretch(1, 1);

        _layout.addWidget(&_sWidget);
        _layout.addItem(&_controlLayout, 1, 0);
        _layout.addWidget(&_loadButton, 2, 0);

        _iterationLayout.addWidget(&_iterationLabel);
        _iterationSlider.setRange(1, 100);
        _iterationSlider.setOrientation(Qt::Horizontal);
        //TODO: declare 5 as constant
        _iterationSlider.setValue(5);
        _iterationLayout.addWidget(&_iterationSlider);

        _layout.addItem(&_iterationLayout, 3, 0);

        _regLayout.addWidget(&_regCheckBox);
        updateRegText();
        _regLambda.setButtonSymbols(QAbstractSpinBox::NoButtons);
        _regLayout.addWidget(&_regLambda);
        _regLambda.setRange(0, 100000);
        _layout.addItem(&_regLayout, 4, 0);

        _updateButton.setEnabled(false);
        _layout.addWidget(&_updateButton, 5, 0);
        _noiseLayout.addWidget(&_noiseCheckBox);
        _noiseLayout.addWidget(&_noiseSpinBox);
        _layout.addItem(&_noiseLayout, 6, 0);

        setLayout(&_layout);

        connect(&_sWidget, &SliceWidget::sliceChanged, this, &ReconstructionViewer::updateStatus);
        connect(&_axisSlider, &QSlider::valueChanged, &_sWidget, &SliceWidget::updateStatus);
        connect(&_loadButton, &QPushButton::pressed, this, &ReconstructionViewer::requestAcquisition);

        connect(&_iterationSlider, &QSlider::valueChanged, this, &ReconstructionViewer::updateStatus);
        connect(&_regCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::updateRegText);

        connect(&_iterationSlider, &QSlider::valueChanged, this, &ReconstructionViewer::enableUpdate);
        connect(&_regCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::enableUpdate);
        connect(&_regLambda, &QDoubleSpinBox::editingFinished, this, &ReconstructionViewer::enableUpdate);
        connect(&_noiseSpinBox, &QDoubleSpinBox::editingFinished, this,
                &ReconstructionViewer::enableUpdate);
        connect(&_noiseCheckBox, &QCheckBox::stateChanged, this,
                &ReconstructionViewer::enableUpdate);
        connect(&_noiseCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::updateSpinBox);

        connect(&_updateButton, &QPushButton::pressed, this, &ReconstructionViewer::updateReconstruction);
        _noiseSpinBox.setMaximum(1000.);
        _noiseSpinBox.setMinimum(0.);
        _noiseSpinBox.setDecimals(2);
        _noiseSpinBox.setSuffix("%");
        _noiseSpinBox.setButtonSymbols(QAbstractSpinBox::NoButtons);
        _noiseSpinBox.setKeyboardTracking(false);
        
        updateRegText();
        updateSpinBox();
        
    }

    void setAcq(Acquisition&& acq){
            _sWidget.setAcq(_regCheckBox.isChecked(),
                            static_cast<float>(_regLambda.value()),
                            _noiseCheckBox.isChecked(),
                            _noiseSpinBox.value()/100,
                            _iterationSlider.value(),
                            std::move(acq));
    }
    
    //on request returns the volume
    std::shared_ptr<const Volume> getRec(){
        return _sWidget.getRec();
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
        _regLambda.setEnabled(_regCheckBox.isChecked());
    }
    
    void updateSpinBox(){
        _noiseSpinBox.setEnabled(_noiseCheckBox.isChecked());
    }

    void updateStatus() {
        _statusView.setText(QString("Current slice: "+QString::number(_sWidget.currSlice()+1)+
                                    " of "+QString::number(_sWidget.slices())));
        _iterationLabel.setText(QString("Iterations: "+QString::number(_iterationSlider.value())));
    }

    void updateReconstruction()
    {
        _sWidget.recParamChanged(_regCheckBox.isChecked(), _regLambda.value(),
                                 _noiseCheckBox.isChecked(), _noiseSpinBox.value()/100,
                                 _iterationSlider.value());
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
        if(!_sWidget.containsRec()){
            return;
        }
        _updateButton.setEnabled(true);
    }
};
