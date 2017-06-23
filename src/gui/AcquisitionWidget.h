#ifndef AQUISITIONWIDGET_H
#define AQUISITIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>

#include "AcquisitionModel.h"
#include "ResultWidget.h"
#include "PoseViewer.h"

class AcquisitionWidget : public QWidget {
    Q_OBJECT
public:
    AcquisitionWidget(std::string path)
        : _aModel(path), layout {}, _pose{&_aModel}, _res {&_aModel}
    {
        _res.setMinimumSize(400, 400);
        QObject::connect(&_aModel, &AcquisitionModel::poseChanged, this, &AcquisitionWidget::update);
        QObject::connect(&_pose, &PoseViewer::sceneRotationChanged, &_res, &ResultWidget::recalcProject);
        layout.addWidget(&_pose);
        layout.addWidget(&_res);
        this->setLayout(&layout);
    }
    
public slots:
    /*
     * It should be possible to directly acess the update of QWidget inside of connect
     * but something didn't quite work, so this is a quick fix
     */
    void update(){
        QWidget::update();
    }

private:
    AcquisitionModel _aModel;
    QHBoxLayout layout;
    PoseViewer _pose;
    ResultWidget _res;    
};

#endif // AQUISITIONWIDGET_H
