#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <QAbstractItemModel>
#include "newrowdialog.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->table->move(0, 0);

    model = new LanguageModel();
    ui->table->setModel(model);

    ui->table->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (model)
    {
        delete model;
    }
}

void MainWindow::on_actionInsert_row_triggered()
{
    NewRowDialog dialog;
    dialog.setModal(true);
    int res = dialog.exec();
    if (res == dialog.Accepted)
    {
        model->append(dialog.NewLanguage);
    }
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    ui->table->resize(size());
    ui->table->setColumnWidth(0, ui->table->width() / 2);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_creator_triggered()
{
    AboutDialog dialog;
    dialog.setModal(true);
    dialog.exec();
}
