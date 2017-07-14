#pragma once

#include <QColor>
#include <QColorDialog>
#include <QPushButton>

/**
 * Button to select a color using the
 * QColorDialog.
 *
 * Code is modified form of the code from:
 * https://stackoverflow.com/questions/18257281/qt-color-picker-widget
 * @brief The SelectColorButton class
 */
class SelectColorButton : public QPushButton
{
    Q_OBJECT
public:
    SelectColorButton(std::initializer_list<QString> l);

    void setColor( const QColor& color );
    const QColor& getColor();

public slots:
    void updateColor();
    void changeColor();

private:
    QColor color;
};
