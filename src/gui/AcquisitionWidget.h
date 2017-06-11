#ifndef AQUISITIONWIDGET_H
#define AQUISITIONWIDGET_H

#include <QWidget>

#include "AcquisitionModel.h"

class AcquisitionWidget : public QWidget {
    Q_OBJECT
public:
    AcquisitionWidget()
    {
    }

    AcquisitionWidget(std::string path)
        : _aModel(path)
    {
    }

    void loadImage(std::string path)
    {
        _aModel.loadImage(path);
    }

private:
    AcquisitionModel _aModel;
};

#endif // AQUISITIONWIDGET_H
