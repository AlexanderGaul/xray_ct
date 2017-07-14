#include "SelectColorButton.h"

SelectColorButton::SelectColorButton(std::initializer_list<QString> l)
{
    typename std::initializer_list<QString>::const_iterator it = l.begin();
    setText(*it);
}

void SelectColorButton::updateColor()
{
    setStyleSheet( "background-color: " + color.name() );
}

void SelectColorButton::changeColor()
{
    QColor newColor = QColorDialog::getColor(color,parentWidget());
    if ( newColor != color )
    {
        setColor( newColor );
    }
}

void SelectColorButton::setColor( const QColor& color )
{
    this->color = color;
    updateColor();
}

const QColor& SelectColorButton::getColor()
{
    return color;
}
