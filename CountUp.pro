#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T14:53:11
#
#-------------------------------------------------

QT += core
QT += gui
QT += sql widgets
QT += sql
QT += opengl


QTPLUGIN += QSQLMYSQL


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CountUp
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    gamemodel.cpp \
    mathnode.cpp \
    login.cpp \
    levelscreen.cpp \
    mainwindow.cpp \
    level.cpp \
    network.cpp

HEADERS  += \
    gamemodel.h \
    mathnode.h \
    login.h \
    levelscreen.h \
    mainwindow.h \
    level.h \
    network.h

FORMS    += \
    login.ui \
    levelscreen.ui \
    mainwindow.ui

LIBS += -lGL -lGLU  -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl

INCLUDEPATH += ../CountUp/Box2D/
LIBS += -L"../CountUp/Box2D/Build/Box2D"
LIBS += -lBox2D

INCLUDEPATH += ../CountUp/usr/include/
LIBS += -L"../CountUp/usr/lib64"
LIBS += -lmysqlcppconn-static


RESOURCES += \
    resources.qrc









