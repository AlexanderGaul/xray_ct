#ifndef TRANSFERFUNCWIDGET_H
#define TRANSFERFUNCWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

#include "TransferFunction.h"

/**
 * Implements a Widget that can change the first linear piece of a given transfer function.
 */

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
    
    // Should be emitted if a reset to the maximum availible intensity of the current volume is desired.
    void requestFunctionReset();

    
};

#endif
