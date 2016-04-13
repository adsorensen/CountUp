#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T14:53:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CountUp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    formula.cpp \
    temp.cpp

HEADERS  += mainwindow.h \
    formula.h \
    temp.h

FORMS    += mainwindow.ui

LIBS += -L"/home/trung/CountUp/SFMLX/lib"

LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

INCLUDEPATH += "/home/trung/CountUp/SFMLX/include"
DEPENDPATH += "/home/trung/CountUp/SFMLX/include"
