#ifndef AQUISITIONWIDGET_H
#define AQUISITIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>

#include "AcquisitionModel.h"
#include "ResultWidget.h"
#include "PoseViewer.h"
#include "Acquisition.h"

class AcquisitionWidget : public QWidget {
    Q_OBJECT
private:
    AcquisitionModel _aModel;
    QHBoxLayout layout;
    PoseViewer _pose;
    ResultWidget _res;

public:
    AcquisitionWidget(std::string path)
        : _aModel(path), layout {}, _pose{_aModel}, _res {_aModel}
    {
        QObject::connect(&_aModel, &AcquisitionModel::poseChanged, this, &AcquisitionWidget::update);
        QObject::connect(&_aModel, &AcquisitionModel::poseChanged, &_res, &ResultWidget::recalcProject);

        /*
         * Without the strech factor of 1 _res takes most of the  width of the widget
         */
        layout.addWidget(&_pose, 1);
        layout.addWidget(&_res, 1);
        this->setLayout(&layout);
    }
    
    Acquisition getAcq() const {
        return _aModel.getAcq();
    }
    
public slots:
    /*
     * It should be possible to directly acess the update of QWidget inside of connect
     * but something didn't quite work, so this is a quick fix
     * TODO
     */
    void update(){
        QWidget::update();
    }

};

#endif // AQUISITIONWIDGET_H
