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
    m_needRefresh(true)
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
}

Diagram::~Diagram()
{
    delete ui;
}

void Diagram::paintEvent(QPaintEvent *event)
{
    //if (m_needRefresh)
    {
        QDialog::paintEvent(event);

        QPainter painter(this);
        //QBrush brush(QColor(150, 120, 200));
        float offset = 0;
        const int SIZE_X = 500, HEIGHT = 30, OFFSET_X = 300 - SIZE_X / 2;
        int sizeY = 250, offsetY = 250 - (sizeY - HEIGHT)/ 2;
        float centerX = OFFSET_X + SIZE_X / 2.0, centerY = offsetY + sizeY / 2.0;
        std::vector<QColor> curUsedColors;
        float curAngles = 0;
        QPointF lastArcPoint(centerX + SIZE_X / 2, centerY);
        for (size_t i = 0; i < langs.size(); ++i)
        {
            QBrush brush(m_needRefresh ? NextColor() : m_usedColors[i]);
            painter.setBrush(brush);
            curUsedColors.push_back(brush.color());
            QPainterPath path;

            float curAngle = 360.0 * langs[i].GetPopulation() / maxPopulation;
    //        float e = 1 - 1.0 * sizeY * sizeY / (SIZE_X * SIZE_X);
            float phi = curAngles * PI / 180.0, phiNew = (curAngles + curAngle) * PI / 180.0;
    //        float a = SIZE_X / 2.0, b = sizeY / 2.0;
    //        float x = (1.0 * a * b ) / sqrtf(1.0 * b * b + 1.0 * a * a * tanf(phi) * tanf(phi));


    //        if (phi > PI / 2.0 && phi < 3 * PI / 2.0)
    //        {
    //            x = -x;
    //        }
    //        float y = (1.0 * a * b ) / sqrtf(1.0 * a * a + 1.0 * b * b /(tanf(phi) * tanf(phi)));
    //        if (phi > PI)
    //        {
    //            y = -y;
    //        }
    //        float x2 = (1.0 * a * b ) / sqrtf(1.0 * b * b + 1.0 * a * a * tanf(phiNew) * tanf(phiNew));
    //        if (phiNew > PI / 2.0 && phiNew < 3 * PI / 2.0)
    //        {
    //            x2 = -x2;
    //        }
    //        float y2 = (1.0 * a * b ) / sqrtf(1.0 * a * a + 1.0 * b * b /(tanf(phiNew) * tanf(phiNew)));
    //        if (phiNew > PI)
    //        {
    //            y2 = -y2;
    //        }

    //        float r = (sizeY * SIZE_X) / (2.0 * (sqrtf(SIZE_X * SIZE_X * sinf(phi) * sinf(phi) + sizeY * sizeY * cosf(phi) * cosf(phi)))),
    //              rNew = (sizeY * SIZE_X) / (2.0 * (sqrtf(SIZE_X * SIZE_X * sinf(phiNew) * sinf(phiNew) + sizeY * sizeY * cosf(phiNew) * cosf(phiNew))));
    //       // qDebug() << cos(phiNew) << phiNew << r << rNew << e;
    //        QPointF points[4] = {
    //            QPointF(centerX + x, centerY - y),
    //            QPointF(centerX + x2, centerY - y2),
    //            QPointF(0, 0),
    //            QPointF(0, 0)
    //        };
    //        points[2].setX(points[1].x());
    //        points[2].setY(points[1].y() - HEIGHT);
    //        points[3].setX(points[0].x());
    //        points[3].setY(points[0].y() - HEIGHT);
            //painter.drawPie(QRect(OFFSET_X, offsetY, SIZE_X, sizeY), offset, curAngle * 16);
    //        if (i < 33)
    //        {
    //            //qDebug() << points[0].x() << points[0].y() << points[1].x() << points[1].y() << x << y << x2 << y2;
    //            //qDebug() << cos(phiNew) << phiNew << r << rNew << e;
    //            //painter.drawPolygon(points, 4);
    //            //    painter.drawLine(points[0], points[1]);

    //        }

            path.moveTo(lastArcPoint);
            path.arcTo(QRectF(OFFSET_X, offsetY, SIZE_X, sizeY), curAngles, curAngle);
            auto start = path.currentPosition();
            lastArcPoint = start;
            //qDebug() << start.x() << start.y();
            //path.lineTo(start.x(), start.y() - HEIGHT);
            path.arcTo(QRectF(OFFSET_X, offsetY - HEIGHT, SIZE_X, sizeY), curAngles + curAngle, -curAngle);
            start = path.currentPosition();
            //qDebug() << start.x() << start.y();
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
        for (size_t i = 0; i < langs.size(); ++i)
        {
            QBrush brush(curUsedColors[i]);
            painter.setBrush(brush);
            float curAngle = 360.0 * langs[i].GetPopulation() / maxPopulation;
            painter.drawPie(QRect(OFFSET_X, offsetY, SIZE_X, sizeY), offset, curAngle * 16);
            offset += curAngle * 16;
        }

        m_needRefresh = false;
    }
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
    this->setWindowTitle(QString::number(event->x()) + " " + QString::number(event->y()));
}
