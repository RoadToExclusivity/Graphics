#ifndef NEWROWDIALOG_H
#define NEWROWDIALOG_H

#include <QDialog>
#include "language.h"

namespace Ui {
class NewRowDialog;
}

class NewRowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewRowDialog(QWidget *parent = 0);
    ~NewRowDialog();

    Language NewLanguage;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewRowDialog *ui;
};

#endif // NEWROWDIALOG_H
