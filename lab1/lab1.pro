#-------------------------------------------------
#
# Project created by QtCreator 2015-09-11T08:27:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab1
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    language.cpp \
    languagemodel.cpp \
    newrowdialog.cpp \
    aboutdialog.cpp \
    aboutwidget.cpp \
    painter.cpp \
    editcommand.cpp

HEADERS  += mainwindow.h \
    language.h \
    languagemodel.h \
    newrowdialog.h \
    aboutdialog.h \
    aboutwidget.h \
    painter.h \
    editcommand.h

FORMS    += mainwindow.ui \
    newrowdialog.ui \
    aboutdialog.ui
