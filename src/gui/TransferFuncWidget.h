#ifndef TRANSFERFUNCWIDGET_H
#define TRANSFERFUNCWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

#include "TransferFunction.h"

class TransferFuncWidget : public QGroupBox
{
    Q_OBJECT
private:
    
    TransferFunction& _function;
    
    QDoubleSpinBox* _iMin;
    QDoubleSpinBox* _iMax;
    QPushButton* _color;
    
    QHBoxLayout* _layout;

    QPushButton* _reset;
    
public:
    TransferFuncWidget(TransferFunction& function);

    void setRange(float from, float to);

private:
    void setIMin(float min);
    void setIMax(float max);
    void changeColor();
    void resetRange();

public slots:
    void changedFunction();

signals:
    void functionChanged();

    void requestFunctionReset();

    
};

#endif
