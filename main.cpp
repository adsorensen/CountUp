#include "mainwindow.h"
#include <QApplication>
#include <login.h>
#include <QObject>
#include <QDebug>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;

    login.show();

    //MainWindow w;
    //w.show();

    return a.exec();
}
