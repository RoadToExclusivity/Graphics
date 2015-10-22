#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    auto q = QLocale::system().name();
    translator.load("lab1_" + q);
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
