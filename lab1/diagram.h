#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QDialog>
#include "languagemodel.h"
#include "language.h"
#include <vector>
#include <QTimer>

namespace Ui {
class Diagram;
}

class Diagram : public QDialog
{
    Q_OBJECT

public:
    explicit Diagram(LanguageModel* model, QWidget *parent = 0);
    ~Diagram();

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    const int SIZE_X = 500, HEIGHT = 30, OFFSET_X = 300 - SIZE_X / 2;
    int sizeY, offsetY;
    float centerX, centerY;

    QPointF lastArcPoint;
    QColor NextColor();
    Ui::Diagram *ui;
    QTimer *m_timer;

    LanguageModel *m_model;
    std::vector<Language> langs;
    std::vector<QColor> m_usedColors;

    int maxPopulation;
    bool m_needRefresh, m_isLeftPressed, m_isRightPressed, m_needAuto;
    float m_angleOffset, m_startAngleMouseDrag, m_startYSize;
    QPoint m_buttonPos;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *) override;

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *);
};

#endif // DIAGRAM_H
