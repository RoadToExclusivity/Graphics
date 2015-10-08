#include "diagram.h"
#include "ui_diagram.h"
#include <QPainter>
#include "math.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>

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
    m_isMidPressed(false),
    m_isRightPressed(false),
    m_needAuto(true),
    m_startAngleMouseDrag(0),
    m_startYSize(0),
    m_timer(new QTimer(this)),
    m_selectedSector(-1),
    m_tipVisible(false),
    m_mouseTipX(-1),
    m_mouseTipY(-1)
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

    if (m_timer)
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));
        m_timer->start(30);
    }
}

Diagram::~Diagram()
{
    delete ui;

    if (m_timer)
    {
        delete m_timer;
    }
}

void Diagram::animate()
{
    if (m_needAuto)
    {
        m_angleOffset += 1;
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

    float curAnglesSum = NormalizeAngle(m_angleOffset);
    QPainterPath testPath;
    testPath.moveTo(QPointF(centerX + SIZE_X / 2, centerY));
    testPath.arcTo(QRectF(OFFSET_X, offsetY, SIZE_X, sizeY), 0, curAnglesSum);
    QPointF lastArcPoint = testPath.currentPosition();

    const QRectF lowArcRectangle(OFFSET_X, offsetY, SIZE_X, sizeY);
    const QRectF highArcRectangle(OFFSET_X, offsetY - HEIGHT, SIZE_X, sizeY);
    std::vector<float> angles;
    for (size_t i = 0; i < langs.size(); ++i)
    {
        QColor color = m_needRefresh ? NextColor() : m_usedColors[i];
        QBrush brush(i == m_selectedSector ? color.lighter() : color);
        painter.setBrush(brush);
        if (m_needRefresh)
        {
            m_usedColors.push_back(color);
        }

        float curAngle = 360.0 * langs[i].GetPopulation() / maxPopulation;
        angles.push_back(curAngle);
        float newAngleSum = curAnglesSum + curAngle;

        if (curAnglesSum >= 0 && curAnglesSum < 180 && NormalizeAngle(newAngleSum) <= 180)
        {
            curAnglesSum = NormalizeAngle(newAngleSum);
            continue;
        }

        QPainterPath path;
        if (curAnglesSum < 180 && newAngleSum > 180)
        {
            path.moveTo(centerX - SIZE_X / 2.0, centerY);
            float newAngle = newAngleSum > 360 ? 180 : (newAngleSum - 180);

            path.arcTo(lowArcRectangle, 180, newAngle);
            auto start = path.currentPosition();
            lastArcPoint = start;
            path.lineTo(start.x(), start.y() - HEIGHT);
            if (newAngleSum > 360)
            {
                path.arcTo(highArcRectangle, 0, -180);
            }
            else
            {
                path.arcTo(highArcRectangle, newAngleSum, -newAngle);
            }

            start = path.currentPosition();
            path.lineTo(start.x(), start.y() + HEIGHT);
        }
        else
        {
            if (curAnglesSum < 360 && newAngleSum >= 360)
            {
                path.moveTo(lastArcPoint);
                float newAngle = curAnglesSum < 180 ? 180 : 360 - curAnglesSum;

                path.arcTo(lowArcRectangle, curAnglesSum, newAngle);
                auto start = path.currentPosition();
                lastArcPoint = start;
                path.lineTo(start.x(), start.y() - HEIGHT);
                if (curAnglesSum < 180)
                {
                    path.arcTo(highArcRectangle, 0, -180);
                }
                else
                {
                    path.arcTo(highArcRectangle, 0, -newAngle);
                }

                start = path.currentPosition();
                path.lineTo(start.x(), start.y() + HEIGHT);
            }
            else
            {
                path.moveTo(lastArcPoint);
                path.arcTo(lowArcRectangle, curAnglesSum, curAngle);
                auto start = path.currentPosition();
                lastArcPoint = start;
                path.lineTo(start.x(), start.y() - HEIGHT);
                path.arcTo(highArcRectangle, newAngleSum, -curAngle);
                start = path.currentPosition();
                path.lineTo(start.x(), start.y() + HEIGHT);
            }
        }

        painter.drawPath(path);
        curAnglesSum = NormalizeAngle(newAngleSum);
    }

    float pieOffset = m_angleOffset * 16;

    for (size_t i = 0; i < m_usedColors.size(); ++i)
    {
        QBrush brush(i == m_selectedSector ? m_usedColors[i].lighter() : m_usedColors[i]);
        painter.setBrush(brush);
        float curPieAngle = angles[i] * 16;
        painter.drawPie(highArcRectangle, pieOffset, curPieAngle);
        pieOffset += curPieAngle;
    }

    if (m_tipVisible)
    {
        painter.setBrush(Qt::black);
        int dx = 50, dy = 100;
        if (m_mouseTipX + dx + 100> size().width())
        {
            dx = size().width() - m_mouseTipX - 105;
        }
        if (m_mouseTipY - dy - 50 < 0)
        {
            dy = m_mouseTipY - 55;
        }

        QPointF startPoint(m_mouseTipX, m_mouseTipY), endPoint(m_mouseTipX + dx, m_mouseTipY - dy);
        painter.drawLine(startPoint, endPoint);
        painter.setBrush(Qt::white);
        QRectF rect(m_mouseTipX + dx, m_mouseTipY - dy - 50, 100, 50);
        painter.drawRect(rect);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 12));
        rect.setX(rect.x() + 5);
        rect.setY(rect.y() + 5);
        painter.drawText(rect, langs[m_selectedSector].GetLanguage() + "\n" +
                         QString::number(langs[m_selectedSector].GetPopulation()));
    }

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
    if (!m_tipVisible && event->button() == Qt::MiddleButton)
    {
        m_isMidPressed = true;
        m_buttonPos = event->pos();
        m_startAngleMouseDrag = m_angleOffset;
        m_needAuto = false;
    }

    if (!m_tipVisible && event->button() == Qt::RightButton)
    {
        m_isRightPressed = true;
        m_buttonPos = event->pos();
        m_startYSize = sizeY;
    }

    if (!m_isMidPressed && !m_isRightPressed && event->button() == Qt::LeftButton)
    {
        int x = event->x(), y = event->y();
        QRgb color = grab().toImage().pixel(x, y);
        bool ok = false;
        for (size_t i = 0; i < m_usedColors.size(); ++i)
        {
            if (m_usedColors[i] == color)
            {
                m_selectedSector = i;
                ok = true;
                break;
            }
        }

        if (!ok)
        {
            m_needAuto = true;
            m_tipVisible = false;
            m_selectedSector = -1;
        }
        else
        {
            m_needAuto = false;
            m_tipVisible = true;
            m_mouseTipX = x;
            m_mouseTipY = y;
        }
    }
}

void Diagram::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        m_isMidPressed = false;
        m_needAuto = !m_tipVisible;
    }

    if (event->button() == Qt::RightButton)
    {
        m_isRightPressed = false;
    }
}

void Diagram::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isMidPressed)
    {
        m_angleOffset = m_startAngleMouseDrag + (event->x() - m_buttonPos.x()) / 5;
        return;
    }

    if (m_isRightPressed)
    {
        int newSizeY = m_startYSize + (event->y() - m_buttonPos.y()) / 3;
        if (newSizeY >= 150 && newSizeY <= 350)
        {
            sizeY = newSizeY;
            offsetY = 250 - (sizeY - HEIGHT)/ 2;
            centerY = offsetY + sizeY / 2.0;
        }

        return;
    }
}

float Diagram::NormalizeAngle(float angle)
{
    if (angle > 360)
    {
        int div = angle / 360;
        angle -= 360 * div;
    }
    else
    {
        if (angle < 0)
        {
            int div = -angle / 360 + 1;
            angle = 360 * div + angle;
        }
    }

    return angle;
}
