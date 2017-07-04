#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QSlider>
#include <QWidget>

#include "SliceWidget.h"

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


public:

    SliceViewer(const Volume& volume)
        : _sWidget(volume)
    {
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
        setLayout(&_layout);

        connect(&_sWidget, &SliceWidget::sliceChanged, this, &SliceViewer::updateStatus);
        connect(&_axisSlider, SIGNAL(valueChanged(int)), &_sWidget, SLOT(updateStatus(int)));
    }


signals:

public slots:    

    void updateStatus()
    {
        _statusView.setText(QString("Current slice: "+QString::number(_sWidget.currSlice()+1)+
                                    " of "+QString::number(_sWidget.slices())));
    }
};
