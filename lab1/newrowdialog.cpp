#include "newrowdialog.h"
#include "ui_newrowdialog.h"

NewRowDialog::NewRowDialog(QWidget *parent) :
    QDialog(parent),
    NewLanguage(Language("", 0)),
    ui(new Ui::NewRowDialog)
{
    ui->setupUi(this);
    QDialog::setFixedSize(size());
}

NewRowDialog::~NewRowDialog()
{
    delete ui;
}

void NewRowDialog::on_buttonBox_accepted()
{
    if (ui->txtLang->text().length() > 0 && ui->spinBox->value() > 0)
    {
        NewLanguage = Language(ui->txtLang->text(), ui->spinBox->value());
    }
}
