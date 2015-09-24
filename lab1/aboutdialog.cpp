#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "aboutwidget.h"
#include <QGridLayout>
#include <QTimer>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
    m_painter(size().height())
{
    ui->setupUi(this);
    setFixedSize(size());

    AboutWidget *widget = new AboutWidget(&m_painter, this);
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(widget, 0, 0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), widget, SLOT(animate()));
    timer->start(50);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    close();
}
