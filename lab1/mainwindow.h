#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include "languagemodel.h"

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

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    Ui::MainWindow *ui;
    LanguageModel *model;
};

#endif // MAINWINDOW_H
