#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include "languagemodel.h"
#include <QUndoStack>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionInsert_row_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_creator_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionShow_diagram_triggered();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void closeEvent(QCloseEvent *);

private:
    void NewFile();
    void OpenFile();
    void SaveFile(QString newFilePath = "");
    void SaveAsFile();
    bool PromptToSave();

    Ui::MainWindow *ui;
    LanguageModel *model;

    const QString APP_NAME = tr("Language statistics");

    QString fileName;
    QString filePath;

    QUndoStack *stack;
};

#endif // MAINWINDOW_H
