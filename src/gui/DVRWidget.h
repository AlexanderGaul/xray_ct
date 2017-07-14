#pragma once

#include <QObject>
#include <QWidget>

#include "VisualizationModel.h"

class DVRWidget : public QWidget
{
    Q_OBJECT
private:
    const VisualizationModel& _model;

public:
    DVRWidget(const VisualizationModel& model)
        : _model(model)
    {
    }

    void paintEvent(QPaintEvent* p_e)
    {

    }

};
