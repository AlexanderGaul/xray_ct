#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QSlider>
#include <QTextEdit>
#include <QWidget>
#include <QPushButton>

#include "SliceWidget.h"
#include "Acquisition.h"

class SliceViewer : public QWidget
{
    Q_OBJECT
private:
    QGridLayout _layout;
    QHBoxLayout _controlLayout;
    QLabel _statusView;
    QGridLayout _sliderLayout;
    QSlider _axisSlider;
    SliceWidget _sWidget;

    QHBoxLayout _iterationLayout;
    QLabel _iterationLabel;
    QSlider _iterationSlider;

    QHBoxLayout _regLayout;
    QCheckBox _regCheckBox;
    QTextEdit _regLambda;
    
    QPushButton _loadButton;

    bool valid(QString text)
    {
        //TODO: Check if text is valid float!
        return true;
    }
    
public:

    SliceViewer() : _layout {}, _controlLayout {}, _statusView {}, _sliderLayout {},
    _axisSlider {}, _sWidget {}, _loadButton {"Load acquisition"}{
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
        _iterationSlider.setRange(1, 10);
        _iterationSlider.setOrientation(Qt::Horizontal);
        _iterationSlider.setValue(_sWidget.iterations());
        _iterationLayout.addWidget(&_iterationSlider);

        _layout.addItem(&_iterationLayout, 3, 0);

        _regLayout.addWidget(&_regCheckBox);
        updateRegText();
        _regLambda.setText("0.0");
        _regLayout.addWidget(&_regLambda);
        _layout.addItem(&_regLayout, 4, 0);
        setLayout(&_layout);

        connect(&_sWidget, &SliceWidget::sliceChanged, this, &SliceViewer::updateStatus);
        connect(&_axisSlider, &QSlider::valueChanged, &_sWidget, &SliceWidget::updateStatus);
        connect(&_loadButton, &QPushButton::pressed, this, &SliceViewer::requestAcquisition);

        connect(&_iterationSlider, &QSlider::valueChanged, this, &SliceViewer::updateStatus);
        connect(&_iterationSlider, &QSlider::valueChanged, this, &SliceViewer::updateReconstruction);

        connect(&_regCheckBox, &QCheckBox::stateChanged, this, &SliceViewer::updateRegText);
        connect(&_regCheckBox, &QCheckBox::stateChanged, this, &SliceViewer::updateReconstruction);
        connect(&_regLambda, &QTextEdit::textChanged, this, &SliceViewer::updateReconstruction);
    }

    void setAcq(Acquisition&& acq){
        QString lambdaText = _regLambda.toPlainText();
        if(valid(lambdaText))
        {
            _sWidget.setAcq(_regCheckBox.isChecked(),
                            lambdaText.toFloat(),
                            _iterationSlider.value(),
                            std::move(acq));
        }
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

    void updateStatus() {
        _statusView.setText(QString("Current slice: "+QString::number(_sWidget.currSlice()+1)+
                                    " of "+QString::number(_sWidget.slices())));
        _iterationLabel.setText(QString("Iterations: "+QString::number(_sWidget.iterations())));
    }

    void updateReconstruction()
    {
        QString lambdaText = _regLambda.toPlainText();
        if(valid(lambdaText))
        {
            _sWidget.recParamChanged(_regCheckBox.isChecked(),
                                     lambdaText.toFloat(),
                                     _iterationSlider.value());
        }
    }
};
