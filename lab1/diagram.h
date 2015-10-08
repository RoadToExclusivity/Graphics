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
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

private:
    float NormalizeAngle(float angle);
    QColor NextColor();

    const int SIZE_X = 500, HEIGHT = 30, OFFSET_X = 300 - SIZE_X / 2;
    int sizeY, offsetY;
    float centerX, centerY;

    Ui::Diagram *ui;
    QTimer *m_timer;

    LanguageModel *m_model;
    std::vector<Language> langs;
    std::vector<QColor> m_usedColors;

    int maxPopulation;
    bool m_needRefresh, m_isMidPressed, m_isRightPressed, m_needAuto;
    float m_angleOffset, m_startAngleMouseDrag, m_startYSize;
    QPoint m_buttonPos;

    int m_selectedSector;
    bool m_tipVisible;
    float m_mouseTipX, m_mouseTipY;
};

#endif // DIAGRAM_H
