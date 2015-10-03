#include "diagram.h"
#include "ui_diagram.h"
#include <QPainter>
#include "math.h"
#include <QDebug>
#include <QMouseEvent>

#define PI 3.14159265

Diagram::Diagram(LanguageModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diagram),
    m_model(model),
    langs(),
    maxPopulation(0),
    m_usedColors(),
    m_needRefresh(true),
    m_angleOffset(0),
    lastArcPoint(),
    m_isLeftPressed(false),
    m_isRightPressed(false),
    m_needAuto(true),
    m_startAngleMouseDrag(0),
    m_startYSize(0)
{
    ui->setupUi(this);
    setFixedSize(size());

    for (int i = 0; i < m_model->rowCount(QModelIndex()); ++i)
    {
        auto indexRow = m_model->index(i, 0);
        auto indexCol = m_model->index(i, 1);
        Language lang(m_model->data(indexRow, Qt::DisplayRole).toString(), m_model->data(indexCol, Qt::DisplayRole).toInt());
        maxPopulation += lang.GetPopulation();
        langs.push_back(lang);
    }

    sizeY = 250;
    m_startYSize = 250;
    offsetY = 250 - (sizeY - HEIGHT)/ 2;
    centerX = OFFSET_X + SIZE_X / 2.0;
    centerY = offsetY + sizeY / 2.0;
    lastArcPoint = QPointF(centerX + SIZE_X / 2, centerY);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));
    m_timer->start(100);
}

Diagram::~Diagram()
{
    delete ui;
}

void Diagram::animate()
{
    if (m_needAuto)
    {
        m_angleOffset += 10;
        if (m_angleOffset >= 360)
        {
            m_angleOffset -= 360;
        }
    }
    this->repaint();
}

void Diagram::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);

    QPainter painter(this);
    float offset = 0;

    std::vector<QColor> curUsedColors;
    float curAngles = m_angleOffset;
    QPainterPath testPath;
    testPath.moveTo(QPointF(centerX + SIZE_X / 2, centerY));
    testPath.arcTo(QRectF(OFFSET_X, offsetY, SIZE_X, sizeY), 0, curAngles);
    lastArcPoint = testPath.currentPosition();
    for (size_t i = 0; i < langs.size(); ++i)
    {
        QBrush brush(m_needRefresh ? NextColor() : m_usedColors[i]);
        painter.setBrush(brush);
        curUsedColors.push_back(brush.color());
        m_usedColors.push_back(brush.color());

        float curAngle = 360.0 * langs[i].GetPopulation() / maxPopulation;
        float phi = curAngles * PI / 180.0, phiNew = (curAngles + curAngle) * PI / 180.0;

        QPainterPath path;
        path.moveTo(lastArcPoint);
        path.arcTo(QRectF(OFFSET_X, offsetY, SIZE_X, sizeY), curAngles, curAngle);
        auto start = path.currentPosition();
        lastArcPoint = start;
        path.arcTo(QRectF(OFFSET_X, offsetY - HEIGHT, SIZE_X, sizeY), curAngles + curAngle, -curAngle);
        start = path.currentPosition();
        path.lineTo(start.x(), start.y() + HEIGHT);
        painter.drawPath(path);

        if (phi < PI && phiNew > PI)
        {
            QPainterPath path;
            path.moveTo(centerX - SIZE_X / 2.0, centerY);
            float newAngle = curAngles + curAngle - 180;
            path.arcTo(QRectF(OFFSET_X, offsetY, SIZE_X, sizeY), 180, newAngle);
            auto start = path.currentPosition();
            //path.lineTo(start.x(), start.y() - HEIGHT);
            path.arcTo(QRectF(OFFSET_X, offsetY - HEIGHT, SIZE_X, sizeY), curAngles + curAngle, -newAngle);
            start = path.currentPosition();
            path.lineTo(start.x(), start.y() + HEIGHT);
            painter.drawPath(path);
        }

        curAngles += curAngle;
        offset += curAngle * 16;
    }
    offsetY -= HEIGHT;
    offset = m_angleOffset * 16;
    for (size_t i = 0; i < langs.size(); ++i)
    {
        QBrush brush(curUsedColors[i]);
        painter.setBrush(brush);
        float curAngle = 360.0 * langs[i].GetPopulation() / maxPopulation;
        painter.drawPie(QRect(OFFSET_X, offsetY, SIZE_X, sizeY), offset, curAngle * 16);
        offset += curAngle * 16;
    }
    offsetY += HEIGHT;
    m_needRefresh = false;
}

QColor Diagram::NextColor()
{
    int r = qrand() % 256, g = qrand() % 256, b = qrand() % 256;
    bool ok = true;
    for (size_t i = 0; i < m_usedColors.size(); ++i)
    {
        QColor color = m_usedColors[i];
        if (qAbs(r - color.red()) + qAbs(g - color.green()) + qAbs(b - color.blue()) <= 8 * 3)
        {
            ok = false;
            break;
        }
    }

    return ok ? QColor(r, g, b) : NextColor();
}

void Diagram::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isLeftPressed = true;
        m_buttonPos = event->pos();
        m_startAngleMouseDrag = m_angleOffset;
        m_needAuto = false;
    }

    if (event->button() == Qt::RightButton)
    {
        m_isRightPressed = true;
        m_buttonPos = event->pos();
        m_startYSize = sizeY;
    }
}

void Diagram::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isLeftPressed = false;
        m_needAuto = true;
    }

    if (event->button() == Qt::RightButton)
    {
        m_isRightPressed = false;
    }
}

void Diagram::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isLeftPressed)
    {
        m_angleOffset = m_startAngleMouseDrag + (event->x() - m_buttonPos.x()) / 8;
    }

    if (m_isRightPressed)
    {
        sizeY = m_startYSize + (event->y() - m_buttonPos.y()) / 6;
        if (sizeY >= 125 && sizeY < 350)
        {
            offsetY = 250 - (sizeY - HEIGHT)/ 2;
            centerX = OFFSET_X + SIZE_X / 2.0;
            centerY = offsetY + sizeY / 2.0;
        }
        else
        {
            sizeY = m_startYSize;
        }
    }
}
