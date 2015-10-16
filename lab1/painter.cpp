#include "painter.h"
#include <QDebug>

Painter::Painter(double maxHeight)
    :m_background(QBrush(QColor(255, 255, 255))),
     m_letter1(QPen(QColor("brown"))),
     m_letter2(QPen(QColor("cyan"))),
     m_time(0),
     m_u(0),
     m_maxHeight(maxHeight),
     m_k(1),
     m_last(0),
     m_curU(0),
     m_y(0)
{
    m_font.setPixelSize(50);

}

void Painter::Paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
    const int LETTER_BOX = 100, LETTER_WIDTH = 70, LETTER_HEIGHT = 80,
              OFFSET_X = (LETTER_BOX - LETTER_WIDTH) / 2, OFFSET_Y = (LETTER_BOX - LETTER_HEIGHT) / 2,
              MID_OFFSET = LETTER_BOX + LETTER_WIDTH / 2;
    int maxY = m_maxHeight - LETTER_HEIGHT - OFFSET_Y;
    painter->fillRect(event->rect(), m_background);

    m_letter1.setWidth(7);
    m_letter2.setWidth(7);
    painter->setPen(m_letter1);

    int diff = elapsed - m_last;
    if (diff < 0)
    {
        diff += 1000;
    }
    m_last = elapsed;

    float k = diff / 1000.0;
    m_time += k;

    float dy = m_k * m_u * m_time + A * m_time * m_time / 2.0;
    if (m_k == -1)
    {
        m_y = maxY + dy;
    }
    else
    {
        m_y = dy;
    }
    m_curU = m_u + m_k * A * m_time;

    if (m_y > maxY)
    {
        m_y = maxY;
        m_u = m_curU;
        m_k = -m_k;
        m_time = 0;
    }
    else
    {
        if (m_y <= 0 && m_k == -1)
        {
            m_u = 0;
            m_k = -m_k;
            m_time = 0;
        }
    }

    painter->drawPie(QRect(-LETTER_WIDTH / 2 + OFFSET_X, m_y + OFFSET_Y, LETTER_WIDTH, LETTER_HEIGHT / 2), -90 * 16, 180 * 16);
    painter->drawPie(QRect(-LETTER_WIDTH / 2 + OFFSET_X, m_y + OFFSET_Y + LETTER_HEIGHT / 2, LETTER_WIDTH, LETTER_HEIGHT / 2), -90 * 16, 180 * 16);

    painter->setPen(m_letter2);
    painter->drawLine(QPointF(MID_OFFSET - 0.75f, m_y + OFFSET_Y), QPointF(MID_OFFSET - 25, m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5));
    painter->drawLine(QPointF(MID_OFFSET + 0.75f, m_y + OFFSET_Y), QPointF(MID_OFFSET + 25, m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5));
    painter->drawLine(QPointF(MID_OFFSET - 40, 0.75f + m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5),
                      QPointF(MID_OFFSET + 40, 0.75f + m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5));
    painter->drawLine(QPointF(MID_OFFSET - 40, 1.5f + m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5),
                      QPointF(MID_OFFSET - 40 + 6, 12 + m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5));
    painter->drawLine(QPointF(MID_OFFSET + 40, 1.5f + m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5),
                      QPointF(MID_OFFSET + 40 - 6, 12 + m_y + OFFSET_Y + 4 * LETTER_HEIGHT / 5));

    painter->setPen(m_letter1);
    painter->drawPie(QRect(200 -LETTER_WIDTH / 2 + OFFSET_X, m_y + OFFSET_Y, LETTER_WIDTH, LETTER_HEIGHT / 2), -90 * 16, 180 * 16);
    painter->drawPie(QRect(200 -LETTER_WIDTH / 2 + OFFSET_X, m_y + OFFSET_Y + LETTER_HEIGHT / 2, LETTER_WIDTH, LETTER_HEIGHT / 2), -90 * 16, 180 * 16);
}
