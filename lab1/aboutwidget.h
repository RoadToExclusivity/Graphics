#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include "painter.h"
#include <QWidget>

class AboutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWidget(Painter *painter, QWidget *parent = 0);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Painter *m_painter;
    int elapsed;
};

#endif // ABOUTWIDGET_H
