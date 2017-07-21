#ifndef BOXSLIDER_H
#define BOXSLIDER_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QSlider>
#include <QString>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSize>

#include <QObject>

class BoxSlider : public QWidget
{
    Q_OBJECT
private:
    QLabel* _label;
    QSlider* _slider;
    QDoubleSpinBox* _box;
public:
    QHBoxLayout* _layout;
public:
    BoxSlider() : BoxSlider(0.f, 1.f, 2, 0.1f){}
    BoxSlider(float min, float max, int decimal, float step);

private:
    void boxChanged(float value);
    void sliderChanged(int value);
    
public slots:
    void changedValue(float value);
signals:
    void valueChanged(float value);

public:
    QSize minimumSizeHint() const override;
};

#endif