#include "VisualizationWidget.h"

#include <iostream>
#include <QGroupBox>
#include <QMessageBox>



VisualizationWidget::VisualizationWidget() :
    _visModel {},
    _loadFileButton {new QPushButton{"Load from file"}},
    _loadRecButton {new QPushButton{"Load reconstruction"}},
    _mprWidget {new MPRWidget{_visModel}},
    _dvrWidget {new DVRWidget{_visModel}},
    _axisWidget {new AxisWidget {_visModel.getMPRModel()}},
    _dvrControlWidget {new DVRControlWidget(_visModel.getDVRModel())},
    _mprControlWidget {new MPRControlWidget(_visModel.getMPRModel())}
{
    ///all layout items for visulisation are composed in the main layout
    QHBoxLayout *mainLayout = new QHBoxLayout {};
    ///manages all menu items
    QGridLayout *menuLayout = new QGridLayout {};
    
    menuLayout->addWidget(_loadFileButton, 0, 0);
    menuLayout->addWidget(_loadRecButton, 0, 1);

    menuLayout->addWidget(_mprControlWidget, 1, 0, 1, 2);
    menuLayout->addWidget(_dvrControlWidget, 2, 0, 1, 2);
    
    mainLayout->addItem(menuLayout);
    QVBoxLayout *mprLayout = new QVBoxLayout {};
    mprLayout->addWidget(_mprWidget);
    mprLayout->addWidget(_axisWidget);
    mprLayout->setStretch(0, 5);
    mprLayout->setStretch(1, 1);
    
    
    QVBoxLayout* dvrLayout = new QVBoxLayout();
    dvrLayout->addWidget(_dvrWidget);
    dvrLayout->setAlignment(Qt::AlignTop);
    
    
    mainLayout->addItem(mprLayout);
    mainLayout->addItem(dvrLayout);
    
    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 5);
    mainLayout->setStretch(2, 5);
    
    mainLayout->setAlignment(Qt::AlignTop);

    setLayout(mainLayout);

    connect(_loadFileButton, &QPushButton::pressed, this, &VisualizationWidget::loadFromFile);
    connect(_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);

    connect(_mprControlWidget, &MPRControlWidget::requestTransferFunctionReset, this, &VisualizationWidget::resetMPRTransferFunction);
    connect(_dvrControlWidget, &DVRControlWidget::requestTransferFunctionReset, this, &VisualizationWidget::resetDVRTransferFunction);
}

void VisualizationWidget::setRec(const std::shared_ptr<const Volume>& vol){
    if(!vol){
        QMessageBox::warning(nullptr, "No Reconstruction Present or Invalid Reconstruction!", "You have to generate a valid reconstruction before it can be visualized!");
        return;
    }
    _visModel.setVolume(*vol);
}

void VisualizationWidget::resetMPRTransferFunction()
{
    _mprControlWidget->setTransferFunctionRange(0, _visModel.volume().maxEntry());
}

void VisualizationWidget::resetDVRTransferFunction()
{
    _dvrControlWidget->setTransferFunctionRange(0, _visModel.volume().maxEntry());
}

void VisualizationWidget::loadFromFile(){
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Medical image data (*.edf)");
    if(filename.isEmpty()){
        return;
    }
    
    try{
        _visModel.setVolume(EDFHandler::read(filename.toStdString()));
    } catch (std::invalid_argument){
        QMessageBox::warning(this, "Unable to Open File", "The file couldn't be read!");
    } catch (std::runtime_error){
        QMessageBox::warning(this, "Unable to Open File", "The file couldn't be read!");
    }
}
