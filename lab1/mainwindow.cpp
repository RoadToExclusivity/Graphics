#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <QAbstractItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->table->move(0, 0);
    model = new LanguageModel();
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
    model->append(Language("Russian", 2000303));
    ui->table->setModel(model);
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    ui->table->resize(size());
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}
