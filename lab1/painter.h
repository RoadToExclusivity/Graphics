#ifndef PAINTER_H
#define PAINTER_H

#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QPen>
#include <QBrush>

class Painter
{
public:
    Painter(double maxHeight);
    void Paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    QBrush m_background;
    QFont m_font;
    QPen m_textPen;
    double m_maxHeight;
    double m_prev;
};

#endif // PAINTER_H
