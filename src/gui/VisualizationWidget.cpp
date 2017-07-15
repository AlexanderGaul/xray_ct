#include "VisualizationWidget.h"

void VisualizationWidget::changeColor()
{
    QColor newColor = QColorDialog::getColor(_visModel.color(),parentWidget());
    if ( newColor != _visModel.color() )
    {
        _visModel.setColor( newColor );        
        updateColorLabel();
    }
}
