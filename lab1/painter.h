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
    QPen m_letter1, m_letter2;
    double m_maxHeight;
    double m_time;
    double m_u;
    double m_curU;
    double m_y;
    int m_k, m_last;

    const double A = 98;
};

#endif // PAINTER_H
