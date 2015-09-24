#include "painter.h"

Painter::Painter(double maxHeight)
    :m_background(QBrush(QColor(255, 255, 255))),
     m_textPen(QPen(Qt::black)),
     m_prev(0),
     m_maxHeight(maxHeight)
{
    m_font.setPixelSize(50);

}

void Painter::Paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
    painter->fillRect(event->rect(), m_background);

    painter->setPen(m_textPen);
    painter->setFont(m_font);
    float k = elapsed / 1000.0;
    painter->drawLine(QPointF(0,m_prev + k * 10), QPointF(0, 100 + m_prev + k * 10));
    painter->drawArc(QRect(0, m_prev + k * 10,100,100 ), 90 * 16, 360);
    //painter->drawText(QRect(0, m_prev + k * 10, 100, 100), Qt::AlignCenter, QStringLiteral("В"));
    painter->drawText(QRect(100, m_prev + k * 5, 100, 100), Qt::AlignCenter, QStringLiteral("Д"));
    painter->drawText(QRect(200, m_prev + k * 100, 100, 100), Qt::AlignCenter, QStringLiteral("В"));

    m_prev += k;
}
