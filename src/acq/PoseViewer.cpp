

void PoseWidget::paintEvent(QPainterEvent* p_e)
{
	QPainter painter(this);

    // xc and yc are the center of the widget's rect.
    qreal xc = width() * 0.5;
    qreal yc = height() * 0.5;

    painter.setPen(Qt::black);

    // draw the cross lines.
    painter.drawLine(xc, rect().top(), xc, rect().bottom());
    painter.drawLine(rect().left(), yc, rect().right(), yc);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::blue);

    // Draw a 13x17 rectangle rotated to 45 degrees around its center-point
    // in the center of the canvas.

    // translates the coordinate system by xc and yc
    painter.translate(xc, yc);

    // then rotate the coordinate system by 45 degrees
    painter.rotate(45);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.
    qreal rx = -(13 * 0.5);
    qreal ry = -(17 * 0.5);
    painter.drawRect(QRect(rx, ry, 2, 50));
}