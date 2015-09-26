#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <QAbstractItemModel>
#include "newrowdialog.h"
#include "aboutdialog.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileName("untitled"),
    filePath(""),
    stack(new QUndoStack(this))
{
    ui->setupUi(this);
    ui->table->move(0, 0);

    model = new LanguageModel(stack, ui->actionSave, this);
    ui->table->setModel(model);

    ui->table->horizontalHeader()->setStretchLastSection(true);

    setWindowTitle(APP_NAME + " - " + fileName);
    ui->actionSave->setEnabled(false);
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

void MainWindow::closeEvent(QCloseEvent*)
{
    if (model->wasChanged)
    {
        if (PromptToSave())
        {
            SaveFile();
        }
    }
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
    if (model->wasChanged)
    {
        if (PromptToSave())
        {
            if (filePath == "")
            {
                SaveAsFile();
            }
            else
            {
                SaveFile();
            }
        }
    }

    fileName = "untitled";
    filePath = "";
    setWindowTitle(APP_NAME + " - " + fileName);
    model->clear();
    model->wasChanged = false;

    stack->clear();
}

void MainWindow::OpenFile()
{
    if (model->wasChanged)
    {
        if (PromptToSave())
        {
            SaveFile();
        }
    }

    auto newFileName = QFileDialog::getOpenFileName(this, tr("Open file"));
    if (newFileName == "")
    {
        return;
    }

    QFile file(newFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen())
    {
        return;
    }

    QString data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (doc.isNull())
    {
        return;
    }

    auto items = doc.object();
    QJsonArray langs = items.value("Language").toArray();
    QJsonArray population = items.value("Population").toArray();

    if (langs.size() != population.size())
    {
        QMessageBox::warning(this, tr("Error"), tr("File is corrupted"));
        return;
    }
    model->clear();

    for (int i = 0; i < langs.size(); ++i)
    {
        Language lang(langs[i].toString(), population[i].toInt());
        model->append(lang);
    }

    filePath = newFileName;
    fileName = newFileName;
    setWindowTitle(APP_NAME + " - " + fileName);
    model->wasChanged = false;
    ui->actionSave->setEnabled(false);
    stack->clear();
}

void MainWindow::SaveFile(QString newFilePath)
{
    if (model->wasChanged || newFilePath != "")
    {
        if (filePath == "" && newFilePath == "")
        {
            SaveAsFile();
            return;
        }

        if (newFilePath == "")
        {
            newFilePath = filePath;
        }

        if (QFileInfo(newFilePath).completeSuffix() != "json")
        {
            newFilePath += ".json";
        }

        QFile file(newFilePath);
        file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

        QJsonObject obj;
        QJsonArray langs, pops;
        for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
        {
            auto indexLang = model->index(i, 0);
            auto indexPop = model->index(i, 1);
            langs.append(model->data(indexLang, Qt::DisplayRole).toString());
            pops.append(model->data(indexPop, Qt::DisplayRole).toInt());
        }

        obj["Language"] = langs;
        obj["Population"] = pops;

        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();

        filePath = newFilePath;
        fileName = filePath;
        setWindowTitle(APP_NAME + " - " + fileName);
        model->wasChanged = false;
        ui->actionSave->setEnabled(false);
    }
}

void MainWindow::SaveAsFile()
{
    auto newFilePath = QFileDialog::getSaveFileName(this, tr("Save file"), QString(), "*.json");
    if (newFilePath != "")
    {
        SaveFile(newFilePath);
    }
}

bool MainWindow::PromptToSave()
{
    auto ans = QMessageBox::question(this, tr("Save file ?"), tr("Do you want to save file ?"), QMessageBox::Yes | QMessageBox::No);
    return (ans == QMessageBox::Yes);
}

void MainWindow::on_actionNew_triggered()
{
    NewFile();
}

void MainWindow::on_actionOpen_triggered()
{
    OpenFile();
}

void MainWindow::on_actionSave_triggered()
{
    SaveFile();
}

void MainWindow::on_actionSave_as_triggered()
{
    SaveAsFile();
}

void MainWindow::on_actionUndo_triggered()
{
    stack->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    stack->redo();
}
