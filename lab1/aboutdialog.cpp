#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "aboutwidget.h"
#include <QGridLayout>
#include <QTimer>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
    m_painter(343),
    m_timer(new QTimer(this))
{
    ui->setupUi(this);
    setFixedSize(size());

    AboutWidget *widget = new AboutWidget(&m_painter, this);
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(widget, 0, 0);

    connect(m_timer, SIGNAL(timeout()), widget, SLOT(animate()));
    m_timer->start(33);
}

AboutDialog::~AboutDialog()
{
    delete ui;

    if (m_timer)
    {
        delete m_timer;
    }
}
