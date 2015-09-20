#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <QAbstractItemModel>
#include "newrowdialog.h"
#include "aboutdialog.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileName("untitled"),
    filePath(""),
    wasChanged(false)
{
    ui->setupUi(this);
    ui->table->move(0, 0);

    model = new LanguageModel(this);
    ui->table->setModel(model);

    ui->table->horizontalHeader()->setStretchLastSection(true);

    this->setWindowTitle(APP_NAME + " - " + fileName);
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
        wasChanged = true;
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

void MainWindow::NewFile()
{
    if (wasChanged)
    {

    }

    fileName = "untitled";
    filePath = "";
    this->setWindowTitle(APP_NAME + " - " + fileName);
    model->clear();
}

bool MainWindow::OpenFile(const QString& fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly, QIODevice::Text);

    QString data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    auto items = doc.object();

    return true;
}

void MainWindow::on_actionNew_triggered()
{
    NewFile();
}

void MainWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
    OpenFile(fileName);
}

void MainWindow::on_actionSave_triggered()
{
    if (filePath == "")
    {

    }
}

void MainWindow::on_actionSave_as_triggered()
{

}
