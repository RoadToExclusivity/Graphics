#include "aboutwidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

AboutWidget::AboutWidget(Painter *painter, QWidget *parent)
    : QWidget(parent), m_painter(painter)
{
    elapsed = 0;
    setFixedSize(parent->size().width(), parent->size().height());
}

void AboutWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void AboutWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    m_painter->Paint(&painter, event, elapsed);
    painter.end();
}
