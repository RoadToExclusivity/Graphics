#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QDialog>
#include "languagemodel.h"
#include "language.h"
#include <vector>

namespace Ui {
class Diagram;
}

class Diagram : public QDialog
{
    Q_OBJECT

public:
    explicit Diagram(LanguageModel* model, QWidget *parent = 0);
    ~Diagram();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QColor NextColor();
    Ui::Diagram *ui;

    LanguageModel *m_model;
    std::vector<Language> langs;
    std::vector<QColor> m_usedColors;

    int maxPopulation;
    bool m_needRefresh;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *) override;
};

#endif // DIAGRAM_H
